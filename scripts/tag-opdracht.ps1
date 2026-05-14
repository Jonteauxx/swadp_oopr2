# =============================================================================
# tag-opdracht.ps1  -  Mijlpaal-tag plaatsen op een voltooide opdracht.
# =============================================================================
# Gebruik:
#   .\scripts\tag-opdracht.ps1 -N 3
#   .\scripts\tag-opdracht.ps1 -N 3 -Beschrijving "meerdere sloten per deur"
#
# Resultaat:
#   - Tag 'opdracht-3' geplaatst op de huidige main HEAD
#   - Pushed naar GitHub zodat het zichtbaar is in de history
#   - Vanaf nu kun je 'git checkout opdracht-3' om die staat te herstellen
# =============================================================================

param(
    [Parameter(Mandatory=$true)]
    [ValidateRange(1,6)]
    [int]$N,

    [string]$Beschrijving = ""
)

$ErrorActionPreference = "Stop"

# --- Check dat we op main zitten ----------------------------------------------
$branch = git branch --show-current
if ($branch -ne "main") {
    Write-Warning "Je staat op '$branch', niet op main."
    Write-Warning "Tags worden meestal op main gezet zodat ze de officiele staat markeren."
    $reply = Read-Host "Toch doorgaan? (y/n)"
    if ($reply -ne "y") { exit 0 }
}

# --- Check geen uncommitted wijzigingen --------------------------------------
# Untracked files (`??`) staan we toe; alleen tracked modifications zijn een
# blokkade voor taggen.
$dirty = git status --porcelain | Where-Object { $_ -notmatch '^\?\?' }
if ($dirty) {
    Write-Error "Je hebt niet-gecommitte wijzigingen aan TRACKED files. Eerst .\deploy.ps1 of stash."
    Write-Host $dirty
    exit 1
}

# --- Tag maken ----------------------------------------------------------------
$tagNaam = "opdracht-$N"
$msg = if ($Beschrijving) {
    "Opdracht $N voltooid: $Beschrijving"
} else {
    "Opdracht $N voltooid"
}

Write-Host ">> tag '$tagNaam' aanmaken: $msg" -ForegroundColor Cyan
git tag -a $tagNaam -m $msg

Write-Host ">> push naar GitHub" -ForegroundColor Cyan
git push origin $tagNaam

Write-Host ""
Write-Host "OK Tag '$tagNaam' staat live." -ForegroundColor Green
Write-Host "    Demo deze opdracht ooit met:" -ForegroundColor DarkGray
Write-Host "      git checkout $tagNaam" -ForegroundColor DarkGray
Write-Host "      .\deploy.ps1 -SkipCommit" -ForegroundColor DarkGray
