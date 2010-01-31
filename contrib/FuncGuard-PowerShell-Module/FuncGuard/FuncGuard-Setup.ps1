# FuncGuard-Setup.ps1

# This script sets up a single usage environment for Notepad++ FuncGuards.
# If you would like FuncGuards to be available whenever you open a PowerShell
# session, copy the contents of the contrib\FuncGuard-PowerShell-Module folder
# to your (My )Documents WindowsPowerShell\Modules folder and adjust your
# PowerShell profile to setup the projects and paths.

<#
  You can run this script from your command line or create a shortcut:

  Target (all on one line):
    C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe -noexit
	-ExecutionPolicy "RemoteSigned"
	-File "C:\PATH_TO_NPPCR_ROOT\contrib\FuncGuard-PowerShell-Module\FuncGuard\FuncGuard-Setup.ps1" -FromShortCut

  Start in:
	PATH_TO_NPPCR_ROOT

  Use the shortcut once, then customize the console preferences to your liking.
#>

param([switch]$FromShortCut)

Push-Location -StackName "FuncGuardSetup"
While (! (Test-Path $(Join-Path -Path $(Get-Location) `
		-ChildPath "notepadPlus.9.sln") -PathType Leaf) ) {
	if ($(Get-Location).ToString() -eq [IO.Path]::GetPathRoot($(Get-Location))) {
		Write-Error "Setup script must be run from within the N++CR solution."
		Pop-Location -StackName "FuncGuardSetup"
		return
	} else {
		Set-Location -Path $(Split-Path -Path $(Get-Location) -Parent)
	}
}

Push-Location -Path $(Join-Path -Path $(Get-Location) `
	-ChildPath "contrib\FuncGuard-PowerShell-Module\") -StackName "FuncGuardSetup"

$Env:PSModulePath = $Env:PSModulePath + ";$(Get-Location)"

if ($(Get-Module -ListAvailable -Name FuncGuard)) {
	Import-Module FuncGuard -Force
} else {
	Write-Error "It seems the FuncGuard module is not available for loading!"
	return
}
Pop-Location -StackName "FuncGuardSetup"

Push-Location -Path $(Join-Path -Path $(Get-Location) `
	-ChildPath "PowerEditor\src\MISC\Debug") -StackName "FuncGuardSetup"

if (! (Test-Path -Path $(Join-Path -Path $(Get-Location) `
		-ChildPath "FuncGuards.h") -PathType Leaf) ) {
	Copy-Item -Path $(Join-Path -Path $(Get-Location) -ChildPath "FuncGuards_skel.h") `
		-Destination $(Join-Path -Path $(Get-Location) -ChildPath "FuncGuards.h") `
}

if ( $(Get-FuncGuardProject -Name "N++"|select -Expand Path) -eq $(Get-Location) ) {
	# Project exists
	Set-FuncGuardProject -Name "N++" | Out-Null
} else {
	$ProjectName = "N++"

	if ( $(Get-FuncGuardProject -Name "N++") ) {
		Write-Warning "A Notepad++ Funcguard Project is already setup with a different path."
		$ProjectName = "N++-$(@((get-location -stackname 'FuncGuardSetup').getenumerator())[1]|split-path -leaf -Leaf)"
		Write-Warning "Attempting to add the new project using the name: $($ProjectName)"
	}

	Add-FuncGuardProject -Name "$ProjectName" -Prefix "NPPCR" -Path "$(Get-Location)"
	Set-FuncGuardProject -Name "$ProjectName" | Out-Null
	Remove-Variable -Name ProjectName -ea 0
}
Pop-Location -StackName "FuncGuardSetup"

Push-Location -Path $(Join-Path -Path $(Get-Location) `
	-ChildPath "scintilla\src\MISC\Debug") -StackName "FuncGuardSetup"

if (! (Test-Path -Path $(Join-Path -Path $(Get-Location) `
		-ChildPath "FuncGuards.h") -PathType Leaf) ) {
	Copy-Item -Path $(Join-Path -Path $(Get-Location) -ChildPath "FuncGuards_skel.h") `
		-Destination $(Join-Path -Path $(Get-Location) -ChildPath "FuncGuards.h") `
}

if ( $(Get-FuncGuardProject -Name "Sci"|select -Expand Path) -eq $(Get-Location) ) {
	# Project exists
} else {
	$ProjectName = "Sci"

	if ( $(Get-FuncGuardProject -Name "Sci") ) {
		Write-Warning "A Notepad++ Funcguard Project is already setup with a different path."
		$NppProjectName = "Sci-$(@((get-location -stackname 'FuncGuardSetup').getenumerator())[1]|split-path -leaf -Leaf)"
		Write-Warning "Attempting to add the new project using the name: $($ProjectName)"
	}

	Add-FuncGuardProject -Name "$ProjectName" -Prefix "NPPCR" -Path "$(Get-Location)"
	Remove-Variable -Name ProjectName -ea 0
}
Pop-Location -StackName "FuncGuardSetup"

Write-Host "`nAvailable FuncGuard Commands...`n"
Get-Command -Module FuncGuard | Select -ExpandProperty Name
Write-Host "`nType 'help <command> for help.`n"

Write-Host "`nAvailable FuncGuard projects:"
Get-FuncGuardProject | select State, Name, Path | Format-Table -AutoSize

if ($FromShortCut) {
	Remove-Variable -Name FromShortCut
} else {
	Pop-Location -StackName "FuncGuardSetup"
}
