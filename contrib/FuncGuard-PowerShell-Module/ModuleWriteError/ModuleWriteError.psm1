################################################################################
## Script Name:     Module Write Error
## Created On:      1/7/2010
## Author:          Thell Fowler
## Tribute:			Joel 'Jaykul' Bennet
## File:            ModuleWriteError.psm1
## Usage:           import-module \Path\to\module\ModuleWriteError.psm1
## Version:         1.0.0
##
## Purpose:         Provides a New-ModuleError function for the creation of an
##					error record that is populated and formatted with an
##					imported module's advanced function information.
##					Generates a module specific custom exception on the fly and
##					can create Error Records using built-in exception types by
##					the short, instead of fully qualified, exception type string.
##
## Reasoning:		Informational error messages generated from 'Write-Error' do
##					not truly represent the current function in some cases and
##					do not inform the user of Module specific details.
##					Usage of $PSCmdlet.WriteError an error record from
##					New-ModuleError can resolve that.
##
## Requirements:    PowerShell Version 2
## 					ModuleWriteError.psd1
##					FunctionInfo.types.ps1xml
## Last Updated:    01/13/2010
## History:
##                  1.0.1  01/13/2010 - Removed nested prompt call remaining from testing.
##                  1.0.0  01/13/2010 - Initial version.
##
##
################################################################################
#requires -version 2.0
Set-StrictMode -Version 2


function New-ModuleExceptionClass{
	[CmdletBinding()]
	param([string]$exceptionType)

# Generic Default Exception Class for use with WriteError output.
$GenericModuleExceptionSource = @"
	using System;
    public class $($exceptionType) : System.ApplicationException
    {
        public $($exceptionType)()
        {
        }
        public $($exceptionType)(string message) : base(message)
        {
        }
        public $($exceptionType)(string message, Exception innerException)
        : base(message, innerException)
        {
        }
    }
"@
	# Suppresses warning: Generated type defines no public methods of it's own.
	Add-Type -TypeDefinition $GenericModuleExceptionSource -IgnoreWarnings -warningaction silentlycontinue
}

function New-ModuleError {
<#
	.SYNOPSIS
		Create an error record that is populated and formatted to provide data
		that is contextual to an imported module's advanced functions.
	.DESCRIPTION
		Informational error messages generated from 'Write-Error' do not truly
		represent the current function in some cases and do not inform the user
		of Module specific details.  When using $PSCmdlet.WriteError though that
		data can be populated into the error record.

		Example output can be viewed by importing ModuleWriteError.Demo.psm1
	.EXAMPLE
		$PSCmdlet.WriteError((
			New-ModuleError `
				"Cannot get `'$Name`' because it does not exist."
		))
	.OUTPUTS
		[ErrorRecord]
#>
	[CmdletBinding()]
	param(
		[Parameter(Position=0, Mandatory=$true)]
		[string]$message,
		[string]$identifierText = $("ErrorNotSpecified"),
		[System.Management.Automation.ErrorCategory]$ErrorCategory = $("NotSpecified"),
		[PSObject]$targetObject,
		[string]$exceptionType = $($null),
		[Exception]$innerException,
		[string]$FQEid = $($null)
	)
	Begin {
		# These are used in building the error record.
		$cmdStack = @((get-pscallstack)|
			foreach {$_|where{
				$_.InvocationInfo.MyCommand -ne $null -and
				$_.Command -ne $PSCmdlet.MyInvocation.MyCommand -and
				$_.Command -ne "prompt"
			}}
		)
		if ( $cmdStack.count -ge 1 ) {
			$MyCommandCaller = (($cmdStack)[0].InvocationInfo.MyCommand)
		} else {
			throw ( New-Object System.InvalidOperationException `
				"New-ModuleError can only be called from a within module function." )
		}

		$MyCallersModule = [regex]::Replace($($MyCommandCaller.ModuleName), "[^\w]", "_")

		if ( $exceptionType.Length -eq 0 ) {
				$exceptionType = "$($MyCallersModule)ModuleException"
		} elseif ( ( "$($exceptionType)" -as [type] -eq $null ) -and
			( $exceptionType -ne "$($MyCallersModule)ModuleException" ) ) {
			$exceptionType =	[AppDomain]::CurrentDomain.GetAssemblies() | %{$_.GetExportedTypes()} |
				?{ $_.Name -eq "$($exceptionType)" -and $_.IsSubclassOf( [System.Exception] ) }
			if ( $exceptionType -as [type] -eq $null ) {
				$exceptionType = "$($MyCallersModule)ModuleException"
			}
		}

		if ( ( $exceptionType -eq "$($MyCallersModule)ModuleException" ) -and
			( "$($exceptionType)" -as [type] -eq $null) ) {
			New-ModuleExceptionClass "$($MyCallersModule)ModuleException"
		}


	}
	Process {
		if ( $FQEid.length -eq 0 ) {
			$FQEid = "$($identifierText), Module : $($MyCommandCaller.ModuleName)\$($MyCommandCaller.InternalName)"
		}
		if ($innerException) {
			$MyErrorRecord = (new-object System.Management.Automation.ErrorRecord(
				(new-object  $exceptionType("$($message)", $innerException)),
				"$($FQEid)",
				$ErrorCategory,
				$targetObject)
			)
		} else {
			$MyException = new-object $exceptionType $message

			$MyErrorRecord = (new-object System.Management.Automation.ErrorRecord(
				$MyException,
				"$($FQEid)",
				$ErrorCategory,
				$targetObject)
			)
		}
		$MyErrorRecord.CategoryInfo.Activity = "$($MyCommandCaller.Name)"

		return $MyErrorRecord

	}
	End {
	}
}

Export-ModuleMember New-ModuleError