# ModuleWriteErrorDemo.psm1

##  Shows how to use the New-ModuleError command with $PSCmdlet.WriteError and
##  provides example output.

function Get-Foo {
	[CmdletBinding()]
	param(
		[string]$name,
		[switch]$interrupt
	)
	Process {
		# Just imagine you did some parsing of a file and the foo object wasn't found.
		$MyErrorRecord = (
			New-ModuleError -ErrorCategory ObjectNotFound `
			-message "Cannot get `'$Name`' because it does not exist." `
			-identifierText FooObjectNotFound -targetObject $($Name)
		)
		$PSCmdlet.WriteError( $MyErrorRecord )
	}
}

function Set-Foo {
	[CmdletBinding()]
	param(
		[string]$name,
		[switch]$interrupt
	)
	Process {
		# we need to get Foo to make sure we can set it.
		get-foo $Name -ev ev_errors	-ea "SilentlyContinue"

		## Trap this module's non-terminating written errors, similar to a catch.
		$ev_errors | sort -Unique |  foreach { $_ |
			where { ($_.exception.getType().Name -eq `
				[regex]::Replace($("$($MyInvocation.MyCommand.ModuleName)ModuleException"), "[^\w]", "_")) -and
				($_.CategoryInfo.Category.ToString() -eq "ObjectNotFound") } |
			%{
				# Do whatever processing would be done to handle the error, or
				# alter the record and rethrow from this function to present the
				# user with an error that matches the function they called.

				# Keep the first part of the FQEid since WriteError will append
				# the current command name.
				$new_FQEid = $([regex]::match($_.FullyQualifiedErrorId,
					"(.*?,){2}").Groups[0].value).ToString().TrimEnd(",")

				# Allow WriteError to correctly apply function naming.

				# This can be done by generating a new exception or by modifying
				# the one created by Get-Foo and re-displaying.

				# The new way:
				$MyErrorRecord = (
					New-ModuleError -ErrorCategory ObjectNotFound `
					-message "Cannot set `'$Name`' because it does not exist." `
					-targetObject $($Name) `
					-innerException $_.Exception -FQEid $($new_FQEid)
				)

				# The modification way:
				#$MyErrorRecord = (
				#	new-object System.Management.Automation.ErrorRecord $MyErrorRecord.Exception,
				#	"$($new_FQI)",
				#	$_.CategoryInfo.Category,
				#	$_.CategoryInfo.TargetName)

				# Modify the message presented to the user so that instead of
				# reporting that we can not 'get' the object when the user called 'set'
				#$MyErrorRecord.ErrorDetails = (
				#	New-Object System.Management.Automation.ErrorDetails `
				#	"Cannot set `'$Name`' because it was not found." )

				# Not sure which is better here...  The activity should present
				# what action caused the error, but should it be the action of
				# this command, or the action of what this command was doing.
				# Neither of these are _really_ needed.
				#$MyErrorRecord.CategoryInfo.Activity = "$($MyPrefixedCommandName)"
				#$MyErrorRecord.CategoryInfo.Activity = $curr_error.CategoryInfo.Activity

				# Finally re-write it from here.
				$Global:Error.removeat(($Global:Error).Indexof($_))
				$PSCmdlet.WriteError( $MyErrorRecord )
			}
		}
	}
}

function Test-foo {
	[CmdletBinding()]
	param()

	Write-Host "`n********************************************************************************" -ForegroundColor Cyan
	$null = $Global:Error.Clear()
	Write-Host "Sample output from Get-Foo call...`n" -ForegroundColor Yellow
	Write-Host "Error message the user sees:" -ForegroundColor Gray
	Get-Foo bar -ev ev_errors
	Write-Host "`nError messages in this step``s error variable `$ev_errors after calling Get-Foo:" -ForegroundColor Gray
	foreach ($err in $ev_errors) {Write-Host ($err|Out-String) -ForegroundColor Red -NoNewline}
	Write-Host "`nDump of what is in `$Global:Error after calling Get-Foo:" -ForegroundColor Gray
	foreach ($err in $Global:Error) {Write-Host ($err|Out-String) -ForegroundColor Red -NoNewline}

	Write-Host "`n********************************************************************************" -ForegroundColor Cyan
	$null = $Global:Error.Clear()
	Write-Host "Sample output from Set-Foo call (which does an internal call to Get-Foo:`n" -ForegroundColor Yellow
	Write-Host "Error message the user sees:" -ForegroundColor Gray
	Set-Foo bar -ev ev_errors
	Write-Host "`nError messages in this step``s error variable `$ev_errors after calling Set-Foo:" -ForegroundColor Gray
	foreach ($err in $Global:Error) {Write-Host ($err|Out-String) -ForegroundColor Red -NoNewline}
	Write-Host "`nDump of what is in `$Global:Error after calling Set-Foo:" -ForegroundColor Gray
	foreach ($err in $Global:Error) {Write-Host ($err|Out-String) -ForegroundColor Red -NoNewline}
}

Import-Module "ModuleWriteError" -Global -ErrorAction Stop
Write-Host "`nDemo module imported:`n`t* Note: This demonstration clears the session`'s `$error collection."
Write-Host "`t* Usage: Run `'Test-Foo`' with various common parameter options and `$ErrorView settings."
Write-Host "`t* Some useful examples for comparison to see what happens to the messages and the error variables:"
Write-Host "`t`t- `$ErrorView=`"NormalView`"; Test-Foo -ea `"Continue`" "
Write-Host "`t`t- `$ErrorView=`"NormalView`"; Test-Foo -ea `"SilentlyContinue`" "
Write-Host "`t`t- `$ErrorView=`"CategoryView`"; Test-Foo -ea `"Continue`" "
Write-Host "`t`t- `$ErrorView=`"CategoryView`"; Test-Foo -ea `"SilentlyContinue`" "
Write-Host "`t`t- Re-import the module using `'-Prefix My`' `n`t`t  Compare errors from `'Test-MyFoo`' vs a direct call to `'Set-MyFoo Bar`'"
Write-Host "`t`t- Rename the module file and import it.`n`t`t  Notice the fully qualified identifier and exception type change to match."
Write-Host "`t* Read the source and notice the different ways to pass and generate module and prefix aware error messages."
Write-Host ""

