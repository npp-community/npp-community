## Just a script to run the psunit tests by prepping the environment and giving
## a few different reporting options.

# See the last line in this file for restoring the bkup
if (! (Test-Path $(Join-Path -Path $($PWD) -ChildPath "FuncGuardTestStarter.ps1"))) {
	Write-Error "This script must be run from the FuncGuard module's root path!"
	if ($(Get-Module -ListAvailable -Name FuncGuard)){
		$moduleroot = (Get-Module -ListAvailable -Name FuncGuard).ModuleBase
		Write-Warning "Based on your module list info the module root may be: $($moduleroot)"
	}
	return
}

Write-Host "The configuration file has been copied to a backup to be restored after testing completes."
$AppData = [System.Environment]::GetFolderPath([System.Environment+SpecialFolder]::LocalApplicationData)
$ModuleConfigPath = join-path -Path $AppData -ChildPath "WindowsPowerShell\Modules\FuncGuard\FuncGuard.config.xml"
Copy-Item -Path $ModuleConfigPath -Destination "$($ModuleConfigPath).PSUnit.bkup" -force -ea 0
Remove-Item -Force $ModuleConfigPath -ea 0
New-Variable -Name ScriptUnderTestRoot -Value $(Get-Location) -Scope Global -Force

# PSUnit makes use of debug mode, which can be helpful at times.
#Set-DebugMode
#Set-ProductionMode

# Just some output controls
$outputToConsole = $true
$doSummary = $false
$failsonly = $false

# Some of the categories from the test file.
$TargetCategory = "All"
#$TargetCategory = "ExportedQUICK"  # Used for debugging a particular test.
#$TargetCategory = "Exported"
#$TargetCategory = "Internal"

$UnitRunScript = Join-Path -Path $PSUnitPath -ChildPath PSUnit.Run.ps1
$ScriptUnderTest = Join-Path -Path $ScriptUnderTestRoot -ChildPath "FuncGuard.test.ps1"
$TestRunnerArgs = "-rawOut $outputToConsole -Category $TargetCategory -PSUnitTestFile `"$ScriptUnderTest`""

$TestRunnerExec = "$UnitRunScript $TestRunnerArgs"
$TestOutput = @(Invoke-Expression -Command $TestRunnerExec)

# Raw-Output and show browser
#$TestRunner = @(%{c:\Users\almostautomated\3rdParty\scripts\PSUnit\psunit-2-beta-1-24978\PSUnit.Run.ps1 -Category:$TargetCategory -PSUnitTestFile "C:\scripts\FuncGuard\FuncGuard.test.ps1" -rawOut -ShowReportInBrowser})

if ($doSummary) {
	$TestOutput | sort Category | group Category |
	%{
		foreach ($category in $_) {
			$CatMeasure=(
				$category.group | select -expand Time | measure-object -ave -sum |
				select Count, Average, Sum
			);
			$CatOutHead="`r`nCategory: $($category.Name)`r`n============================";
			Write-output $CatOutHead;
			$category.group |
			sort @{Expression={[regex]::Match($_.Test,"^Test.*?_")}}, Time |
			group @{Expression={[regex]::Match($_.Test,"^Test.*?_")}} |
			%{
				foreach ($item in $_) {
					ft -input $item.group Result, Time, Test, Reason -auto -Hide | Write-Output
				};
				$TestMeasure =(
					$item.group | select -expand Time | measure-object -ave -sum |
					select Count, Average, Sum `
				);
				$itemName=(	$item.Name.ToString().TrimEnd("_") );
				$testOut = "`r`nTest Summary for $($itemName.Substring(5)):"
				$testOut += "`tCount:`t$($TestMeasure.Count)"
				$testOut += "`t`tSum:`t$($TestMeasure.sum)"
				$testOut += "`t`tAverage:`t$($TestMeasure.Average)"
				Write-Output "$($testOut)"
			}
		};
		$CatFoot = "Category summary for $($category.Name):"
		$CatFoot += "`tCount:`t$($CatMeasure.Count)"
		$CatFoot += "`t`tSum:`t$($catMeasure.sum)"
		$CatFoot += "`t`tAverage:`t$($catMeasure.Average)"
		Write-Output "$($CatFoot)`r`n"
	} | Write-Output

	#} | ft -auto # End command measure.
} elseif ($failsonly) {
	$TestOutput | where {$_.Result -eq "Fail"} | sort Test -Descending | ft -AutoSize
} else {
	$TestOutput | sort Category | group Category | foreach {$_.Group| sort Test } |
		ft -groupby Category -Property `
			@{"Name"="Time"; "Expression" = {$_.Time.tostring().substring(0,$_.Time.tostring().Indexof("."))}; "Alignment" = "Right"}, `
			@{"Name"="Result"; "Expression" = {$_.Result}; "Alignment" = "Center"}, `
			@{"Name" = "Test"; "Expression" = {$_.Test.toString().substring(5)}}, `
			Reason -AutoSize -wrap
	$TestOutput | where {$_.Result -eq "Pass"} | Measure-Object | select -expand count | %{if ($_ -gt 0){"Tests Passed: $($_)"|Write-Output}}
	$TestOutput | where {$_.Result -eq "Fail"} | Measure-Object | select -expand count | %{if ($_ -gt 0){"Tests Failed: $($_)"|Write-Host -ForegroundColor Red}}
	$TestOutput | Measure-Object Result | select -expand count | %{"Tests Run   : $($_)`n"|Write-Output}
}

Write-Host "Restoring the backup configuration file"
Move-Item -Destination $ModuleConfigPath -Path "$($ModuleConfigPath).PSUnit.bkup" -force
remove-variable -Name ScriptUnderTestRoot -Scope Global -Force
Write-Host "Please remove-module FuncGuard and re-import after testing.`n"
