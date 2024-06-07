import subprocess

def open_powershell():
    subprocess.Popen(['powershell', '-Command', 'git status; git add .; git status; git commit -m "Date $(Get-Date -Format yyyy-MM-dd)"; git status; git push origin master; git status; clear-history'])

# Example usage:
if __name__ == "__main__":
    open_powershell()
