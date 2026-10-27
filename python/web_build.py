"""Prepare and collect an isolated, disposable web firmware build."""
import argparse
import base64
import gzip
import io
import json
import os
from pathlib import Path
import re
import shutil

ROOT = Path(__file__).resolve().parents[1]
MAX_BYTES = 240_000
FILES = {'config.h', 'keymap.c', 'keymap_common.h', 'keymap_rgb.c', 'rules.mk', 'vial.json'}


def target_for(task_id):
    if not re.fullmatch(r'[a-f0-9]{8}-[a-f0-9]{4}-4[a-f0-9]{3}-[89ab][a-f0-9]{3}-[a-f0-9]{12}', task_id):
        raise ValueError('Invalid task identifier')
    return 'web-' + task_id


def prepare(task_id, payload):
    target = target_for(task_id)
    if len(payload) > 58_000:
        raise ValueError('Configuration payload is too large')
    compressed = base64.b64decode(payload, validate=True)
    with gzip.GzipFile(fileobj=io.BytesIO(compressed)) as stream:
        raw = stream.read(MAX_BYTES + 1)
    if len(raw) > MAX_BYTES:
        raise ValueError('Configuration expands beyond the limit')
    files = json.loads(raw)
    if not isinstance(files, dict) or set(files) != FILES:
        raise ValueError('Unexpected configuration files')
    if any(not isinstance(value, str) or '\x00' in value for value in files.values()):
        raise ValueError('Configuration must be UTF-8 text')
    vial = json.loads(files['vial.json'])
    rows, cols = vial['matrix']['rows'], vial['matrix']['cols']
    if type(rows) is not int or type(cols) is not int or not 1 <= rows <= 16 or not 1 <= cols <= 32:
        raise ValueError('Invalid matrix dimensions')
    if 'customKeycodes' in vial or 'zorokbOptions' in vial:
        raise ValueError('Unsupported Vial fields')
    folder = ROOT / 'keyboard' / target
    folder.mkdir(exist_ok=False)
    try:
        makefile = (ROOT / 'keyboard/zero64/Makefile').read_text()
        makefile = makefile.replace('$(shell PWD)', '$(CURDIR)')
        (folder / 'Makefile').write_text(makefile)
        for name, content in files.items():
            (folder / name).write_text(content, encoding='utf-8')
    except Exception:
        shutil.rmtree(folder)
        raise
    output = os.environ.get('GITHUB_OUTPUT')
    if output:
        with open(output, 'a') as stream:
            stream.write(f'keyboard={target}\n')
    print(f'Prepared isolated build {target}')


def collect(task_id):
    target = target_for(task_id)
    output = ROOT / '.web-output' / task_id
    output.mkdir(parents=True, exist_ok=True)
    for suffix in ('bin', 'hex'):
        base = ROOT / 'build' / target if suffix == 'hex' else ROOT / 'build'
        matches = list(base.glob(f'{target}-*.{suffix}'))
        if len(matches) != 1 or not 0 < matches[0].stat().st_size <= 4 * 1024 * 1024:
            raise ValueError(f'Missing or invalid {suffix} firmware')
        shutil.copyfile(matches[0], output / f'firmware.{suffix}')
    print('Collected HEX and BIN firmware')


def cleanup(task_id):
    target = target_for(task_id)
    shutil.rmtree(ROOT / 'keyboard' / target, ignore_errors=True)
    shutil.rmtree(ROOT / 'build' / target, ignore_errors=True)
    for suffix in ('bin', 'uf2'):
        for path in (ROOT / 'build').glob(f'{target}-*.{suffix}'):
            path.unlink()
    # Runner teardown also discards .web-output after upload.
    print('Temporary configuration and build files removed')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('command', choices=['prepare', 'collect', 'cleanup'])
    parser.add_argument('--task-id', required=True)
    arguments = parser.parse_args()
    if arguments.command == 'prepare':
        payload = os.environ.get('WEB_CONFIG_GZIP')
        if payload is None:
            event = json.loads(Path(os.environ['GITHUB_EVENT_PATH']).read_text())
            payload = event['inputs']['config_gzip']
        prepare(arguments.task_id, payload)
    elif arguments.command == 'collect':
        collect(arguments.task_id)
    else:
        cleanup(arguments.task_id)
