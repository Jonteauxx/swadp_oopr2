#!/usr/bin/env bash
# =============================================================================
# pi-update.sh - wordt door deploy.ps1 op de Pi uitgevoerd na een push.
# =============================================================================
# Stappen:
#   1. git pull (fast-forward only)
#   2. cmake configureren als build/ nog niet bestaat
#   3. cmake build (alle cores)
#   4. ctest (alle unit-tests draaien, output bij falen)
# =============================================================================

set -euo pipefail

REPO_DIR="${HOME}/swadp_oopr2"
cd "${REPO_DIR}"

BRANCH="$(git branch --show-current)"
echo ">> branch:       ${BRANCH}"
echo ">> git fetch + reset --hard origin/${BRANCH}"
# Deploy-target altijd in sync met origin; geen lokale wijzigingen verwacht.
git fetch origin
git reset --hard "origin/${BRANCH}"

if [ ! -d build ]; then
    echo ">> eerste keer: cmake configureren"
    cmake -B build -S .
fi

echo ">> cmake --build build"
cmake --build build -j"$(nproc)"

echo ""
echo ">> unit-tests draaien (ctest)"
cd build
if ctest --output-on-failure; then
    echo ""
    echo "============================================================"
    echo "  BUILD OK + TESTS GESLAAGD"
    echo "  executable: ${REPO_DIR}/build/gebouw"
    echo "============================================================"
else
    echo ""
    echo "============================================================"
    echo "  BUILD OK, maar TESTS GEFAALD - zie hierboven"
    echo "============================================================"
    exit 1
fi
