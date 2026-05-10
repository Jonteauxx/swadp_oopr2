#!/usr/bin/env bash
# =============================================================================
# pi-update.sh - wordt door deploy.ps1 op de Pi uitgevoerd na een push.
# =============================================================================
# Doet:
#   1. git pull (fast-forward only - geen merges per ongeluk)
#   2. cmake configureren als build/ nog niet bestaat
#   3. cmake build met alle cores
# =============================================================================

set -euo pipefail

REPO_DIR="${HOME}/swadp_oopr2"
cd "${REPO_DIR}"

echo ">> branch:       $(git branch --show-current)"
echo ">> git pull --ff-only"
git pull --ff-only

if [ ! -d build ]; then
    echo ">> eerste keer: cmake configureren"
    cmake -B build -S .
fi

echo ">> cmake --build build"
cmake --build build -j"$(nproc)"

echo ""
echo "============================================================"
echo "  BUILD OK - executable: ${REPO_DIR}/build/gebouw"
echo "============================================================"
