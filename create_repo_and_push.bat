@echo off
echo ========================================
echo Creating SKSE for Modlists Repository
echo ========================================
echo.

cd /d C:\Users\cashc\skse64

echo Step 1: Creating GitHub repository...
gh repo create cashcon57/skse-for-modlists --public --description "Performance-optimized SKSE64 for large modlists (Nolvus v6, 400+ plugins)"

if errorlevel 1 (
    echo Repository may already exist, continuing...
)

echo.
echo Step 2: Pushing code to GitHub...
git push -u fork master

echo.
echo Step 3: Pushing release tag...
git push fork v2.0.20-optimized-1

echo.
echo ========================================
echo SUCCESS!
echo ========================================
echo.
echo Repository: https://github.com/cashcon57/skse-for-modlists
echo.
echo Opening in browser...
gh repo view cashcon57/skse-for-modlists --web

echo.
pause
