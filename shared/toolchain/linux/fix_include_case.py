#!/usr/bin/env python3
"""Normaliza los #include "..." de FuetEngine para sistemas con mayusculas/minusculas:
  - cambia '\\' por '/'
  - ajusta cada componente de la ruta a la mayuscula real del fichero en disco.
Solo toca includes con comillas que se pueden resolver (relativos al fichero o a una raiz de includes).
Uso: fix_include_case.py [--write] RAIZ_INCLUDE [DIR_A_PROCESAR ...]
Sin --write solo informa (ensayo)."""
import os, re, sys

EXTS = {'.h', '.hpp', '.inl', '.c', '.cpp', '.cc'}
INC = re.compile(r'^(\s*#\s*include\s*)"([^"]+)"(.*)$')


def resolve(base, rel):
    """Resuelve rel (con '/') desde base ignorando mayusculas. Devuelve la ruta con la mayuscula real o None."""
    parts = []
    cur = base
    for comp in rel.split('/'):
        if comp in ('', '.'):
            continue
        if comp == '..':
            cur = os.path.dirname(cur)
            parts.append('..')
            continue
        try:
            names = os.listdir(cur)
        except OSError:
            return None
        if comp in names:
            real = comp
        else:
            m = [n for n in names if n.lower() == comp.lower()]
            if len(m) != 1:
                return None
            real = m[0]
        parts.append(real)
        cur = os.path.join(cur, real)
    return '/'.join(parts) if os.path.exists(cur) else None


def main():
    args = [a for a in sys.argv[1:] if a != '--write']
    write = '--write' in sys.argv
    root = os.path.abspath(args[0])
    targets = [os.path.abspath(a) for a in args[1:]] or [root]
    changed_files = changed_lines = unresolved = 0
    for t in targets:
        for dp, _, fns in os.walk(t):
            for fn in fns:
                if os.path.splitext(fn)[1].lower() not in EXTS:
                    continue
                path = os.path.join(dp, fn)
                with open(path, 'rb') as f:
                    raw = f.read()
                try:
                    text = raw.decode('utf-8')
                    enc = 'utf-8'
                except UnicodeDecodeError:
                    text = raw.decode('latin-1')
                    enc = 'latin-1'
                out, dirty = [], False
                for line in text.split('\n'):
                    eol = '\r' if line.endswith('\r') else ''
                    body = line[:-1] if eol else line
                    m = INC.match(body)
                    if m:
                        rel = m.group(2).replace('\\', '/')
                        new = resolve(dp, rel) or resolve(root, rel)
                        if new is None:
                            unresolved += 1  # externo (sistema/sdk) o inexistente: no se toca
                        elif new != m.group(2):
                            body = '%s"%s"%s' % (m.group(1), new, m.group(3))
                            dirty = True
                            changed_lines += 1
                    out.append(body + eol)
                if dirty:
                    changed_files += 1
                    if write:
                        with open(path, 'wb') as f:
                            f.write('\n'.join(out).encode(enc))
    print('%s: %d lineas en %d ficheros; %d includes sin resolver (externos)' %
          ('modificado' if write else 'ensayo', changed_lines, changed_files, unresolved))


if __name__ == '__main__':
    main()
