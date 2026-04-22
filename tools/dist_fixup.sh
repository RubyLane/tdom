#!/bin/sh
# Freshen manifest.uuid in the meson-dist tree so versionhash.h
# reflects the commit actually being released.  git archive exports
# whatever was committed to HEAD for manifest.uuid, which may be stale
# if later commits didn't touch it.  Overwrite with the current HEAD.
set -eu

cd "$MESON_DIST_ROOT"

# The on-disk manifest.uuid may be a symlink in the source tree; remove
# it before writing so we end up with a plain file in the tarball.
rm -f manifest.uuid

git -C "$MESON_SOURCE_ROOT" rev-parse HEAD | tr -d '\n\r' > manifest.uuid
