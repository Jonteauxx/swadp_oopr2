# =============================================================================
# run-pi.ps1  -  Start de gebouw-applicatie op de Pi via VNC display.
# =============================================================================
# Gebruik:
#   .\scripts\run-pi.ps1
# =============================================================================
# DISPLAY=:0 zorgt dat het venster op het VNC-scherm verschijnt zodat je
# het via VNC Viewer op je Windows-laptop ziet.

param(
    [string]$PiHost = "rpi@192.168.137.62",
    [string]$Display = ":0"
)

Write-Host ">> start gebouw op Pi (DISPLAY=$Display)" -ForegroundColor Cyan
ssh $PiHost "DISPLAY=$Display ~/swadp_oopr2/build/gebouw"
