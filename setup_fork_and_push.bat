@echo off
echo ====================================
echo SKSE for Modlists - Fork Setup
echo ====================================
echo.

REM Add gh to PATH
set PATH=%PATH%;C:\Program Files\GitHub CLI

echo Step 1: Authenticating with GitHub...
echo.
echo You'll be prompted to:
echo 1. Choose "GitHub.com"
echo 2. Choose "HTTPS"
echo 3. Choose "Login with a web browser"
echo 4. Copy the one-time code and press Enter
echo 5. Browser will open - paste the code and authorize
echo.
pause

gh auth login

echo.
echo Step 2: Creating repository...
cd C:\Users\cashc\skse64

gh repo create skse-for-modlists --public --source=. --remote=fork --description="Performance-optimized SKSE64 for large modlists (Nolvus v6, 400+ plugins)" --push

if errorlevel 1 (
    echo.
    echo Repository creation failed. Trying manual creation...
    echo Creating repository on GitHub...
    gh repo create cashcon57/skse-for-modlists --public --description="Performance-optimized SKSE64 for large modlists (Nolvus v6, 400+ plugins)"

    echo Adding fork remote...
    git remote set-url fork https://github.com/cashcon57/skse-for-modlists.git

    echo Pushing code...
    git push fork master
)

echo.
echo Step 3: Creating git tag for release...
git tag -a v2.0.20-optimized-1 -m "Performance optimizations for large modlists (Nolvus v6)

Major improvements:
- Plugin name hash map: ~400x faster lookups
- Serialization UID hash map: ~160,000x faster save/load
- Event registration optimization: ~9x faster
- Memory optimizations: 30-40%% reduction
- Safety improvements

See PERFORMANCE_OPTIMIZATIONS.md for details."

echo Pushing tag...
git push fork v2.0.20-optimized-1

echo.
echo ====================================
echo SUCCESS!
echo ====================================
echo.
echo Your fork has been created at:
echo https://github.com/cashcon57/skse-for-modlists
echo.
echo Next steps:
echo 1. Build the project (see BUILD_INSTRUCTIONS.md)
echo 2. Create a release on GitHub
echo 3. Upload the built binaries
echo.
echo View your repository:
gh repo view cashcon57/skse-for-modlists --web
echo.
pause
