################################################################################
## Script Name:     N++CR FuncGuard Module
## Created On:      12/19/2009
## Author:          Thell Fowler
## File:            FuncGuard.psm1
## Usage:           import-module \Path\to\module\FuncGuard.psm1
## Version:         0.0.1.1
## Purpose:         Provides cmdlets for working with function guards.
##					- Implemented:
##                      Get-FuncGuard - list available func_guards categories.
##                      Enable-FuncGuard
##                      Disable-FuncGuard
##                      Add-FuncGuard - Add func_guards to header and source.
##                      Remove-FuncGuard - Delete entries for function guard.
##						Update-FuncGuard - update category template headers.
##
## Requirements:    PowerShell Version 2
## History:
##					0.0.1.1  01/27/2010 - Altered Update-FuncGuardFiles to use ShouldProcess
##					0.0.1.0  01/26/2010 - Submitted for code review.
##					0.0.0.17 01/23/2010 - Continue FuncGuardProject implementation.
##					0.0.0.16 01/22/2010 - Implement LoadModuleConfig usage.
##					0.0.0.15 01/21/2010 - Start FuncGuardProject implementations.
##					0.0.0.14 01/19/2010 - Refactoring of Add and remove functions.
##					0.0.0.13 01/17/2010 - Implement Update-FuncGuard.
##					0.0.0.12 01/15/2010 - Implement modified ModuleWriteError.
##					0.0.0.11 01/13/2010 - Implement ModuleWriteError usage.
##                  0.0.0.10 01/02/2010 - Start Remove-FuncGuard implementation.
##                  0.0.0.9  01/01/2010 - Continue Add-FuncGuard implementation.
##                  0.0.0.8  01/01/2010 - Start Disable-FuncGuard implementation.
##                  0.0.0.7  12/30/2009 - Start Enable-FuncGuard implementation.
##                  0.0.0.6  12/26/2009 - Start ps1xml implementation.
##                  0.0.0.5  12/24/2009 - Start Get-FuncGuard implementation.
##                  0.0.0.4  12/21/2009 - Implement getFuncGuard file functions.
##                  0.0.0.3  12/21/2009 - Implement parameter testing functions.
##                  0.0.0.2  12/20/2010 - Start Add-FuncGuard implementation.
##                  0.0.0.1  12/19/2009 - Initial rendition/outline.
##
##
################################################################################
#requires -version 2.0
Set-StrictMode -Version 2

################################################################################
## Exported functions.
################################################################################
function New-FuncGuardProject {
<#
	.SYNOPSIS
		Creates the target C++ code files for a new function guard project.
	.DESCRIPTION
		Function guards are a N++CR debug tool created by Jocelyn Legault for debug
		function tracing.  New-FuncGuardProject generates the required source code
		lines for function guard definition, enable/disable control, and importing.
	.EXAMPLE
		New-FuncGuardProject -Name "Notepad++CR" -Prefix "NPPCR" `
			-path "$NPPCR_ROOT\PowerEditor\src\MISC\Debug\"

		Results in the following base files being created in the path:
			FuncGuards_skel.h - version controlled non-active header.  This
				file is the one that would be committed to your version control
				system for other developers to have access to.

			FuncGuards.h - active header that is used by your coding project.
			    This file should be ignored in your version control system and
				is used to store the details of function guards including,
					- usage
					- description
					- enabled disabled macro definition

			FuncGuards.cpp - version controlled macro definition for each defined
				function guard.  Using the value from the active header your
				project will either enable or disable a particular function guard
				category.

			FuncGuardsImport.h - version controlled file that lists each macro in
				a single header that is part of your projects pre-compiled headers
				or at the top of each file requiring function guards.  This makes
				all defined function guards available to the file.

	.PARAMETER Name
		Used to identify the project within the configuration settings.

	.PARAMETER Prefix
		The default prefix used in the C++ Preprocessor MACRO.

	.PARAMETER Path
		Path to the function guard source files directory. Will default to current path.

	.PARAMETER Force
		Override normal parameter validation for project name.  Typically
		this is just used internally to initialize built-in configurations loaded
		from the module.
#>
	[CmdletBinding(DefaultParameterSetName="Validate")]

	PARAM(
		[Parameter(ParameterSetName="Validate",Mandatory=$false,Position=0)]
		[Switch]$Validate
	,
		[Parameter(ParameterSetName="Force",Mandatory=$false,Position=0)]
		[Switch]$Force
	,
		[Parameter(ParameterSetName="Validate", Mandatory=$true, Position=1,
			HelpMessage="`nName used to identify the project within the configuration settings.")]
		[Parameter(ParameterSetName="Force", Position=0, Mandatory=$true,
			HelpMessage="`nName used to identify the project within the configuration settings.")]
		[ValidateScript({
			($FuncGuardSettings.("Configurations").containskey("$_") -eq $false) -or
			($Force)
		})]
		[string]
		$Name = $(Throw 'A unique project name is required.')
	,
		[Parameter(Position=2, Mandatory=$true,
			HelpMessage="`nThe default prefix used in the C++ Preprocessor MACRO.")]
		[ValidateScript({testPrefixParam $_})]
		[string]
		$Prefix = $(Throw 'Preprocessor MACRO prefix is required.')
	,
		[Parameter(Position=3, Mandatory=$true,
			HelpMessage="`nPath where FuncGuard skeleton, header, source and import files will be created.")]
		[string]
		$Path = $(Get-Location)
	)
	Process {
		$TargetFiles = @("FuncGuards.h", "FuncGuards_skel.h", "FuncGuards.cpp","FuncGuardsImport.h")
		foreach ($file in $TargetFiles) {
			$TargetPath = Join-Path -Path $Path -ChildPath $file
			if (Test-Path -Path $TargetPath -PathType Leaf)  {
				throw ((
					New-ModuleError -ErrorCategory ResourceExists `
					-message "Cannot create `'$($TargetPath)`' because it already exists.`n Please remove the file or select a different path." `
					-identifierText FuncGuardFileExistsError -targetObject $($TargetPath)
				))
			}
		}
		$SourceRoot = Split-Path -Parent $PSCmdlet.MyInvocation.MyCommand.Module.Path
		If (! $(Test-Path $Path -PathType Container) ) {
			$null = New-Item -Path $Path -ItemType Directory
		}

		foreach ($file in $TargetFiles) {
			switch ($file) {
				"FuncGuards.h"{
					$SourceName = "FuncGuards_skel.h.orig"
				}
				"FuncGuards_skel.h"{
					$SourceName = "FuncGuards_skel.h.orig"
				}
				"FuncGuards.cpp"{
					$SourceName = "FuncGuards.cpp.orig"
				}
				"FuncGuardsImport.h"{
					$SourceName = "FuncGuardsImport.h.orig"
				}
			}
			$Source = Join-Path -Path $SourceRoot -ChildPath $SourceName
			$Destination = Join-Path $Path -ChildPath $file
			Copy-Item  -Force -Path $Source -Destination $Destination
		}

		if ($PSCmdlet.ParameterSetName -ne "Force") {
			$Configuration=@{"Name" = $Name; "Path" = $Path; "Prefix" = $Prefix; "Type" = "Custom"}
			$FuncGuardSettings.("Configurations").add($Name, $Configuration)
			$FuncGuardSettings.export()
		}
	}
}
function Add-FuncGuardProject {
<#
	.SYNOPSIS
		Add Function Guard project information for already existing projects.
	.DESCRIPTION
		Add-FuncGuardProject stores a project's settings including Name, Path,
		and Prefix.  If the project files do not already exist they will be
		created.

	.EXAMPLE
		Add-FuncGuardProject -Name "Notepad++CR" -Prefix "NPPCR" `
			-path "$NPPCR_ROOT\PowerEditor\src\MISC\Debug\"

	.PARAMETER Name
		Used to identify the project within the configuration settings.

	.PARAMETER Prefix
		The default prefix used in the C++ Preprocessor MACRO.

	.PARAMETER Path
		Path to the function guard source files directory. Will default to current path.
#>
	[CmdletBinding()]
	param(
		[Parameter(Position=0, Mandatory=$true,
			HelpMessage="`nName used to identify the project within the configuration settings.")]
		[ValidateScript({ $FuncGuardSettings.("Configurations").containskey("$_") -eq $false })]
		[string]
		$Name = $(Throw 'A unique project name is required.')
	,
		[Parameter(Position=0, Mandatory=$true,
			HelpMessage="`nThe default prefix used in the C++ Preprocessor MACRO.")]
		[ValidateScript({testPrefixParam $_})]
		[string]
		$Prefix = $(Throw 'Preprocessor MACRO prefix is required.')
	,
		[Parameter(Position=0, Mandatory=$true,
			HelpMessage="`nPath where FuncGuard skeleton, header, source and import files will be created.")]
		[ValidateScript({Test-Path $_ -PathType Container})]
		[string]
		$Path = $(Get-Location)
	)
	Process {
		$TargetFiles = @("FuncGuards.h", "FuncGuards_skel.h", "FuncGuards.cpp","FuncGuardsImport.h")
		foreach ($file in $TargetFiles) {
			$testPath = Join-Path -Path $Path -ChildPath $file
			if (!  (Get-ChildItem -Path $testPath -ea 0)) {
				try {
					New-FuncGuardProject -Name $Name -Prefix $Prefix -Path $Path
				}
				catch {
					throw ((
						New-ModuleError -ErrorCategory InvalidOperation `
						-message "File states are mixed.  Some required files exist in and some don't exist in $($Path)" `
						-identifierText FuncGuardFileStateError -targetObject $($file)
					))
				}
			}
		}

		if ( $FuncGuardSettings.("Configurations").containskey($Name) -eq $false ) {
			$Configuration=@{"Name" = $Name; "Path" = $Path; "Prefix" = $Prefix; "Type" = "Custom"}
			$FuncGuardSettings.("Configurations").add($Name, $Configuration)
			$FuncGuardSettings.export()
		}
	}
}
function Get-FuncGuardProject {
<#
	.SYNOPSIS
		Displays Function Guard project information.
	.DESCRIPTION
		Get-FuncGuardProject retrieves and displays the	project settings,
		including the default paths and prefix values.
	.EXAMPLE
		Get-FuncGuardProject -Name PSUnit

	.PARAMETER Name
		Name of the FuncGuard project to display.
#>
	[CmdletBinding()]
	param(
		[Parameter(Position=0, Mandatory=$false,
			HelpMessage="`nName of the FuncGuard project to get.")]
		[string]
		$Name = $("*")
	)
	Process {
		$ConfigSet = $FuncGuardSettings.("Configurations")
		foreach ($ConfigKey in $ConfigSet.keys) {
			if ($ConfigKey -like $Name) {

				$ConfigurationState = "Inactive"
				if ( $ConfigKey -eq $FuncGuardSettings.("ActiveConfiguration") ) {
					$ConfigurationState = "Active"
				}

				$ModuleConfiguration = new-object psobject

				foreach ($Setting in $ConfigSet.item($ConfigKey).keys) {
					Add-Member -inputobject $ModuleConfiguration `
						-MemberType NoteProperty `
						-Name $Setting `
						-Value $($ConfigSet.item($ConfigKey).item($Setting))
				}

				Add-Member -inputobject $ModuleConfiguration `
					-MemberType NoteProperty `
					-Name "State" `
					-Value $ConfigurationState

				$ModuleConfiguration.psobject.Typenames.add('FuncGuardProjectInfo')
				Write-Output $ModuleConfiguration
			}
		}
	}
}
function Set-FuncGuardProject {
<#
	.SYNOPSIS
		Displays Function Guard project information.
	.DESCRIPTION
		Function guards are a N++CR debug tool created by Jocelyn Legault for debug
		function tracing.  Get-FuncGuardProject retrieves and displays the
		project settings, including the default paths and prefix values.
	.EXAMPLE
		Get-FuncGuardProject -Name PSUnit

	.PARAMETER Name
		Name of the FuncGuard project to display.
#>
	[CmdletBinding()]
	param(
		[Parameter(Position=0, Mandatory=$true,
			HelpMessage="`nName of the FuncGuard project to get.")]
		[ValidateScript({$FuncGuardSettings.("Configurations").containskey("$_")})]
		[string]
		$Name = $(Throw 'A project name is required.')
	)
	Process {
		if ( $FuncGuardSettings.("ActiveConfiguration") -ne $Name ) {
			$FuncGuardSettings.("ActiveConfiguration") = $Name
			$FuncGuardSettings.export()
			Write-Output -InputObject $(Get-FuncGuardProject $Name)
		}
	}
}
function Remove-FuncGuardProject {
<#
	.SYNOPSIS
		Removes Function Guard project information from the configuration file.
	.DESCRIPTION
		Function guards are a N++CR debug tool created by Jocelyn Legault for debug
		function tracing.  Remove-FuncGuardProject removes project data values
		from the configuration file and module settings variable.  It will not
		remove the files from their paths.  If you remove the project information
		and then decide to add it back, use the New-FuncGuardProject's -Force
		parameter.
	.EXAMPLE
		Remove-FuncGuardProject -Name PSUnit

	.PARAMETER Name
		Name of the FuncGuard project to remove.
#>
	[CmdletBinding()]
	param(
		[Parameter(Position=0, Mandatory=$false,
			HelpMessage="`nA project name is required.")]
		[ValidateScript({$FuncGuardSettings.("Configurations").containskey("$_")})]
		[string]
		$Name
	)
	Process {
		If ( $FuncGuardSettings.("Configurations").$($Name).Type -eq "BuiltIn" ) {
			throw ((
				New-ModuleError -ErrorCategory InvalidOperation `
				-message "$($Name) is a builtin module configuration and cannot be removed!" `
				-identifierText FuncGuardProjectError -targetObject $($Name)
			))
		} elseif ( $FuncGuardSettings.("ActiveConfiguration") -eq $Name ) {
			throw ((
				New-ModuleError -ErrorCategory InvalidOperation `
				-message "$($Name) is currently set as the active project and cannot be removed!" `
				-identifierText FuncGuardProjectError -targetObject $($Name)
			))
		}

		$FuncGuardSettings.("Configurations").remove($($Name))
		$FuncGuardSettings.export()
	}
}
function Add-FuncGuard {
<#
	.SYNOPSIS
		Modifies the target C++ source code for a new function guard.
	.DESCRIPTION
		Function guards are a N++CR debug tool created by Jocelyn Legault for debug
		function tracing.  Add-FuncGuard generates the required source code lines
		for function guard definition, enable/disable control, and importing.
	.EXAMPLE
		PS C:\NPPCR_ROOT>Add-FuncGuard -prefix "NPPCR" -path "$NPPCR_ROOT\PowerEditor\src\MISC\Debug\" `
		>> -name "WinMain" `
		>> -description "Trace functions in WinMain.cpp, should not be used in other files."

		Results in the following code being added:
			... to FuncGuards_skel.h and FuncGuards.h (if it exists):
				// func_guard(guard_WinMain);
				// Description:  Trace functions in WinMain.cpp, should not be used in other files.
				// #define NPPCR_GUARD_WINMAIN

			... to FuncGuards.cpp:
				#ifdef NPPCR_GUARD_WINMAIN
					func_guard_enable_cat(guard_WinMain);
				#else
					func_guard_disable_cat(guard_WinMain);
				#endif

			... to FuncGuardsImport.h
				func_guard_import_cat(guard_WinMain);

	.PARAMETER Prefix
		An all upper case string prefixing the preprocessor MACRO value.
		Defaults to the currently active FuncGuard Project prefix.
	.PARAMETER Path
		Path to the function guard source files directory.
		Defaults to the currently active FuncGuard Project path.
	.PARAMETER Name
		CamelCase_UnderscoreSpaced name of the function guard category.
	.PARAMETER Description
		Category notes inserted as comments to FuncGuards_skel.h && FuncGuards.h
#>
	[CmdletBinding()]
	param(
		[Parameter(Position=0, Mandatory=$true,
			HelpMessage="`nCamelCase_UnderscoreSpaced function guard name to add.")]
		[ValidateScript({testNameParam $_})]
		[string]
		$Name = $(Throw 'CamelCase_UnderscoreSpaced function guard name is required.')
	,
		[Parameter(Position=1, Mandatory=$True,
			HelpMessage="`nA description comment that will be added to the main function guard header.")]
		[string]
		$Description
	,
		[Parameter(
			HelpMessage="`nPrefix used in the C++ Preprocessor MACRO.")]
		[ValidateScript({testPrefixParam $_})]
		[string]
		$Prefix = $(Get-FuncGuardProjectPrefix)
	,
		[Parameter(
			HelpMessage="`nPath to the FuncGuard header, source and import header.")]
		[ValidateScript({testPathParam $_})]
		[string]
		$Path = $(Get-FuncGuardProjectPath)
	)
	BEGIN {
		$OFS = [Environment]::NewLine
		$FuncGuardFiles = Get-FuncGuardSourceFiles -Source -SourcePath $Path
		if ($FuncGuardFiles.item("ActiveHeader") -eq $Null) {
			throw (New-Object IO.FileNotFoundException $Path"\FuncGuards.h")
		}

	}
	PROCESS {
		$UpdateSkeleton = $false
		if (! (Get-FuncGuard -Name $Name -Path $Path -ea 0) ) {
			$NewHeaderContent = normalizeLineEnding(
					createHeaderContent -Name $Name `
						-Prefix $Prefix -Description $Description
				)
			$NewSourceContent = normalizeLineEnding(
					createSourceContent -Prefix $Prefix -Name $Name
				)
			$NewImporterContent = normalizeLineEnding(
					createImporterContent -Name $Name
				)

			$FuncGuards = @(Get-FuncGuard -Path $Path -ea 0)
			if ($FuncGuards.count) {
				$PrevGuard = 0..($FuncGuards.Count - 1) |
					Where { $FuncGuards[$_].Name -lt $Name } | select -First 1
				$NextGuard = 0..($FuncGuards.Count - 1) |
					Where { $FuncGuards[$_].Name -gt $Name } | select -First 1
			} else {
				$PrevGuard = $null
				$NextGuard = $null
			}

			if ( ($PrevGuard -eq $null) -and ($NextGuard -ne $null)) {
				#Prepend spacing
				$NewHEaderContent = $OFS + $NewHeaderContent + $OFS
				$NewSourceContent = $NewSourceContent + $OFS + $OFS
				$NewImporterContent = $NewImporterContent + $OFS
			} elseif ( ($PrevGuard -ne $null) -and ($NextGuard -eq $null) ) {
				# Append spacing
				$NewHEaderContent = $OFS + $NewHeaderContent + $OFS
				$NewSourceContent = $OFS + $NewSourceContent + $OFS
				$NewImporterContent = $NewImporterContent + $OFS
			} elseif ( ($PrevGuard -ne $null) -and ($NextGuard -ne $null) ) {
				# Insert Spacing
				$NewHEaderContent = $OFS + $OFS + $NewHEaderContent
				$NewSourceContent = $NewSourceContent + $OFS + $OFS
				$NewImporterContent = $NewImporterContent + $OFS
			} else {
				# First guard in file!
				$NewHEaderContent = $OFS + $NewHeaderContent + $OFS
				$NewSourceContent = $OFS + $NewSourceContent + $OFS
				$NewImporterContent = $OFS + $NewImporterContent + $OFS
			}

			if ( $NextGuard -ne $null ) {
				$guard = $FuncGuards[$NextGuard]
				# Since header code changes when enabled/disabled use actual code.
				$HeaderIndexToken = $guard.rawCodeBlock
				$SourceIndexToken = normalizeLineEnding(
						createSourceContent -Prefix $guard.HeaderPrefix `
							-Name $guard.Name
					)
				$ImporterIndexToken = normalizeLineEnding(
						createImporterContent -Name $guard.Name
					)
			} else {
				$HeaderIndexToken = $OFS + "#endif // SHIPPING"
				$SourceIndexToken = $OFS + "// FUNCGUARD_BLOCK_END"
				$ImporterIndexToken = $OFS + "#endif // MISC_DEBUG_FUNCGUARDS_H"
			}

			foreach ($key in $FuncGuardFiles.keys) {
				$ContentModified = $false
				$NewFileText = $null
				[string]$FileText = Get-Content $FuncGuardFiles.item($key)

				switch ($key) {
					"ActiveHeader" {
						$NewFileText = $FileText.Insert(
							$FileText.IndexOf($HeaderIndexToken),$NewHeaderContent)
						$ContentModified = $true
					}
					"GuardSource" {
						$NewFileText = $FileText.Insert(
							$FileText.IndexOf($SourceIndexToken),$NewSourceContent)
						$ContentModified = $true
					}
					"GuardImport" {
						$NewFileText =$FileText.Insert(
							$FileText.IndexOf($ImporterIndexToken),$NewImporterContent)
						$ContentModified = $true
					}
				}

				if ($ContentModified) {
					$UpdateSkeleton = $true
					Set-Content -Path $FuncGuardFiles.item($key) `
						-Value $NewFileText
				}
			}
		} else {
			$PSCmdlet.WriteError((
				New-ModuleError -ErrorCategory ResourceExists `
				-message "Cannot add `'$Name`' because it already exists.`n" `
				-identifierText FuncGuardExistsError -targetObject $($Name)
			))
		}
	}
	END {
		if ($UpdateSkeleton) {
			Update-FuncGuardFiles -Skeleton -Path $Path -wa 0
		}
	}
}
function Get-FuncGuard {
<#
	.SYNOPSIS
		Retrieve defined function guards from C++ source code.
	.DESCRIPTION
		Function guards are a N++CR debug tool created by Jocelyn Legault for debug
		function tracing.  Add-FuncGuard generates the required source code lines to
		for function guard definition, enable/disable control, and importing.
	.EXAMPLE
		PS C:\NPPCR_ROOT>Get-FuncGuard -path "$NPPCR_ROOT\PowerEditor\src\MISC\Debug\"
	.OUTPUTS
		FuncGuard object
	.PARAMETER Path
		Path to the function guard source files directory.
		Defaults to the active FuncGuard Project path.
	.PARAMETER Name
		Name pattern to search for.  Wildcards supported.
	.PARAMETER Skeleton
		Use FuncGuards_skel.h instead of trying the current FuncGuards.h.
#>
	[CmdletBinding()]
	param(
		[Parameter(Position=0, Mandatory=$False,
			HelpMessage="`nName of the function guard. Wildcards supported.")]
		[string]
		$Name = "*"
	,
		[Parameter(Mandatory=$False,
			HelpMessage="`nFuncGuard header or skeleton file. Defaults to Funcguards.h")]
		[ValidateScript({Test-Path -Path $_})]
		[string]
		$Path = $(Get-FuncGuardProjectPath)
	,
		[Parameter(Position=2, Mandatory=$False)]
		[switch]
		$Skeleton
	)
	Begin{
		$regex=[regex] "(?msx)
			# Code Block
			(?<CodeBlock>
			.+?
			# Usage and Name
			(?<Usage>func_guard\(guard(?<Name>\p{Lu}\w+)\);)
			.+?

			# Description
			Description:\s+(?<Description>.+?)
			\s*(\s*|/{2,})+\s*\#define.+?

			# Header Macro
			(?<Macro>FUNCGUARD_(?<Prefix>[\w-[\p{Ll}\p{P}]]+)[\w-[\p{Ll}]]+)
			)
		"
	}
	Process{
		if ( Test-Path -Path $Path -PathType Container ) {
			try {
				if ($Skeleton) {
					$Path = Join-Path -Path $Path -ChildPath "FuncGuards_skel.h"
				} else {
					$Path = Join-Path -Path $Path -ChildPath "FuncGuards.h"
				}
			}
			catch {
				throw ( new-object IO.FileNotFoundException $Path )
			}
		}

		if (! ( Select-String -Path $Path `
			-Pattern "^\#define.*_DEBUG_FUNCGUARD_H" -List ) ) {
			Write-Error "Invalid file!  Skipping: $Path"
			continue
		}

		$StartLine = (Select-String $Path `
			-Pattern "^\s*[#]ifndef\s+SHIPPING" -list | select -expand LineNumber)
		$EndLine = (Select-String $Path `
			-Pattern "^\s*[#]endif\s+//\s*SHIPPING" -list | select -expand LineNumber)
		if ( ($StartLine -eq $null) -or ($Endline -eq $null) ) {
			throw (New-Object System.IO.InvalidDataException $Path)
		}
		$OFS = [Environment]::NewLine
		[string] $HeaderContentText = (Get-Content $Path)[$StartLine .. $EndLine]
		#$HeaderContentText = [string]::Join("\r\n", $HeaderContent)

		$nameMatchFound = $false
		[regex]::Matches($HeaderContentText, $regex)|foreach {
			if ($_.Groups["Name"].Value -like $Name) {
				if ( $_.Groups["Description"].Value ) {
					$FullDescription= $_.Groups["Description"].Value
					$ShortDescription = ($FullDescription -split $OFS)[0]

				} else {
					$ShortDescription = ""
					$FullDescription = ""
				}

				$regexState = "//\s*\#define\s*"
				$regexState += $_.Groups["Macro"].Value
				$match = [regex]::Match($_.Groups["CodeBlock"].Value, "$regexState")
				if ( $match.Success -eq $true ) {
					$Status = "Disabled"
				} else {
					$Status = "Enabled"
				}

				$FuncGuard = New-Object PSObject -Property @{
					Name				= $_.Groups["Name"].Value
					Usage				= $_.Groups["Usage"].Value
					HeaderMacro			= $_.Groups["Macro"].Value
					HeaderPrefix		= $_.Groups["Prefix"].Value
					Status				= $Status
					ShortDescription	= $ShortDescription
					FullDescription		= $FullDescription
					RawCodeBlock		= $_.Groups["CodeBlock"].Value
				}
				$FuncGuard.psobject.Typenames.add('FuncGuardInfo')

				Write-Output $FuncGuard
				$nameMatchFound = $true
			}
		}
		if ( $nameMatchFound -ne $true ) {
			if ( $Name -eq "*" ) {
				$PSCmdlet.WriteError((
					New-ModuleError -ErrorCategory ObjectNotFound `
					-message "It appears that no function guards have been set for the active project.`n" `
					-identifierText FuncGuardNotFoundError -targetObject $($Name)
				))
			} else {
				$PSCmdlet.WriteError((
					New-ModuleError -ErrorCategory ObjectNotFound `
					-message "Cannot get `'$Name`' because it does not exist.`n" `
					-identifierText FuncGuardNotFoundError -targetObject $($Name)
				))
			}
		}
	}
	End{
	}
}
function Enable-FuncGuard {
<#
	.SYNOPSIS
		Enable defined function guards in C++ source code.
	.DESCRIPTION
		Function guards are a N++CR debug tool created by Jocelyn Legault for debug
		function tracing.  Enable-FuncGuard removes leading comment markers from
		the C++ preprocessor MACRO definitions in the active FuncGuards.h file.
	.PARAMETER Path
		Path to the function guard source files directory.
	.PARAMETER Name
		Name pattern to search for.  Wildcards supported.
	.OUTPUTS
		NPPCR.FuncGuard
	.EXAMPLE
		PS C:\NPPCR_ROOT>Enable-FuncGuard Class1 -path "$NPPCR_ROOT\PowerEditor\src\MISC\Debug\"
#>
	[CmdletBinding()]
	param(
		[Parameter(Position=0, Mandatory=$False,
			HelpMessage="`nName of the function guard. Wildcards supported.")]
		[string]
		$Name = "*"
	,
		[Parameter(Mandatory=$False,
			HelpMessage="`nFuncGuard header or skeleton file. Defaults to Funcguards.h")]
		[ValidateScript({Test-Path -Path $_})]
		[string]
		$Path = $(Get-FuncGuardProjectPath)
	)
	Begin{
	}
	Process {
		$Path = getHeaderPathFromPathParam $Path
		$FuncGuardText = Get-Content -Path $Path
		$doSetContent = $false

		Get-FuncGuard -Path $Path -Name $Name -ea 0 -ev gfg_errors |
			foreach {
				if ( ($_.Name -like $Name) -and ($_.Status -eq "Disabled") ) {
					$regexMacro = "^\s*//(/)*\s*\#define\s+"
					$regexMacro += $_.HeaderMacro
					$regexMacro += "\s*$"
					$replacementString = "#define "
					$replacementString += $_.HeaderMacro
					$FuncGuardText = foreach ($line in $FuncGuardText) {
						$line -replace "$regexMacro", "$replacementString"
					}
					$doSetContent = $true
					$_.Status = "Enabled"
				}
				Write-Output $_
			}

		$gfg_errors | sort -Unique |  foreach { $_ |
			where { ($_.exception.getType().Name -eq `
				[regex]::Replace($("$($MyInvocation.MyCommand.ModuleName)ModuleException"), "[^\w]", "_")) -and
				($_.CategoryInfo.Category.ToString() -eq "ObjectNotFound") } |
			%{
				$new_FQEid = $([regex]::match($_.FullyQualifiedErrorId,
					"(.*?,){2}").Groups[0].value).ToString().TrimEnd(",")
				$PSCmdlet.WriteError((
					New-ModuleError -ErrorCategory ObjectNotFound `
					-message "Cannot enable `'$Name`' because it does not exist.`n" `
					-targetObject $($Name) `
					-innerException $_.Exception -FQEid $($new_FQEid)
				))
				$Global:Error.removeat(($Global:Error).Indexof($_))
			}
		}
	}
	End {
		if ($doSetContent) {
			Set-Content -Path $Path -Value $FuncGuardText
		}
	}
}
function Disable-FuncGuard {
<#
	.SYNOPSIS
		Disable defined function guards in C++ source code.
	.DESCRIPTION
		Function guards are a N++CR debug tool created by Jocelyn Legault for debug
		function tracing.  Disable-FuncGuard adds leading comment markers to
		the C++ preprocessor MACRO definitions in the active FuncGuards.h file.
	.EXAMPLE
		PS C:\NPPCR_ROOT>Disable-FuncGuard Class1* -path "$NPPCR_ROOT\PowerEditor\src\MISC\Debug\"

	.PARAMETER Path
		Path to the function guard header or skeleton file.
		Defaults to the active Function Guard project path.
	.PARAMETER Name
		Name pattern to search for.  Wildcards supported.
#>
	[CmdletBinding()]
	param(
		[Parameter(Position=1, Mandatory=$False,
			HelpMessage="`nName of the function guard. Wildcards supported.")]
		[string]
		$Name = "*"
	,
		[Parameter(Mandatory=$False,
			HelpMessage="`nPath to the FuncGuard header.  `
			Defaults to the active Function Guard project's Funcguards.h")]
		[ValidateScript({Test-Path -Path $_})]
		[string]
		$Path = $(Get-FuncGuardProjectPath)
	)
	Begin{
	}
	Process {
		$Path = getHeaderPathFromPathParam $Path
		$FuncGuardText = Get-Content -Path $Path
		$doSetContent = $false

		Get-FuncGuard -Path $Path -Name $Name -ea 0 -ev gfg_errors |
		foreach {
			if ( ($_.Name -like $Name) -and ($_.Status -eq "Enabled") ) {
				$regexMacro = "^\s*\#define\s+"
				$regexMacro += $_.HeaderMacro
				$regexMacro += "\s*$"
				$replacementString = "//#define "
				$replacementString += $_.HeaderMacro
				$FuncGuardText = foreach ($line in $FuncGuardText) {
					$line -replace "$regexMacro", "$replacementString"
				}
				$doSetContent = $true
				$_.Status = "Disabled"
			}
			Write-Output $_
		}

		$gfg_errors | sort -Unique |  foreach { $_ |
			where { ($_.exception.getType().Name -eq `
				[regex]::Replace($("$($MyInvocation.MyCommand.ModuleName)ModuleException"), "[^\w]", "_")) -and
				($_.CategoryInfo.Category.ToString() -eq "ObjectNotFound") } |
			%{
				$new_FQEid = $([regex]::match($_.FullyQualifiedErrorId,
					"(.*?,){2}").Groups[0].value).ToString().TrimEnd(",")
				$PSCmdlet.WriteError((
					New-ModuleError -ErrorCategory ObjectNotFound `
					-message "Cannot disable `'$Name`' because it does not exist.`n" `
					-targetObject $($Name) `
					-innerException $_.Exception -FQEid $($new_FQEid)
				))
				$Global:Error.removeat(($Global:Error).Indexof($_))
			}
		}
	}
	End {
		if ($doSetContent) {
			Set-Content -Path $Path -Value $FuncGuardText
		}
	}
}
function Remove-FuncGuard {
<#
	.SYNOPSIS
		Remove the source code blocks for function guards from C++ source code
		and active header.
	.DESCRIPTION
		Function guards are a N++CR debug tool created by Jocelyn Legault for debug
		function tracing.  Remove-FuncGuard removes the source code lines from
		the function guard definition, enable/disable control, and importing.
		This function will not remove function guards directly from the skeleton
		or template headers.
		Use Update-FuncGuard to update the skeleton abd template headers.
	.EXAMPLE
		PS C:\NPPCR_ROOT>Remove-FuncGuard -path "$NPPCR_ROOT\PowerEditor\src\MISC\Debug\ -Name Class1"
	.PARAMETER Path
		Path to the function guard source files directory.
		Defaults to the active Function Guard project path.
	.PARAMETER Name
		Name pattern to search for.  Wildcards supported.
#>
	[CmdletBinding()]
	param(
		[Parameter(Position=1, Mandatory=$True,
			HelpMessage="`nName of the function guard. Wildcards supported.")]
		[string]
		$Name
	,
		[Parameter(Mandatory=$False,
			HelpMessage="`nPath to the FuncGuard header.  `
			Defaults to the active Function Guard project's Funcguards.h")]
		[ValidateScript({Test-Path -Path $_})]
		[string]
		$Path = $(Get-FuncGuardProjectPath)
	)
	Begin{
		$OFS = [Environment]::Newline
		try {
			$FuncGuardFiles = Get-FuncGuardSourceFiles -Source -SourcePath $Path
		}
		catch {
			throw $_
		}
		if ($FuncGuardFiles.item("ActiveHeader") -eq $Null) {
			throw (New-Object IO.FileNotFoundException $Path"\FuncGuards.h")
		}
	}
	Process{
		$FuncGuard = @(Get-FuncGuard -Path $FuncGuardFiles.item("ActiveHeader"))
		$FuncGuardRemoved = $null

		foreach ($guard in $FuncGuard) {
			if ( $guard.Name -like $Name ) {
				# Since header code changes when enabled/disabled use actual code.
				$HeaderIndexToken = $guard.rawCodeBlock
				$SourceIndexToken = normalizeLineEnding(
						createSourceContent -Prefix $guard.HeaderPrefix `
							-Name $guard.Name
					)
				$ImporterIndexToken = normalizeLineEnding(
						createImporterContent -Name $guard.Name
					)

				$PrevGuard = 0..($FuncGuard.Count - 1) |
					Where { $FuncGuard[$_].Name -lt $Name } | select -First 1
				$NextGuard = 0..($FuncGuard.Count - 1) |
					Where { $FuncGuard[$_].Name -gt $Name } | select -First 1

				if ( ($PrevGuard -eq $null) -and ($NextGuard -ne $null)) {
					# Prepended spacing
					$HeaderIndexToken = $OFS + $HeaderIndexToken
					$SourceIndexToken = $SourceIndexToken + $OFS + $OFS
					$ImporterIndexToken = $ImporterIndexToken + $OFS
				} elseif ( ($PrevGuard -ne $null) -and ($NextGuard -eq $null) ) {
					# Appended spacing
					# No padding needed for $HeaderIndexToken
					$SourceIndexToken = $OFS + $SourceIndexToken + $OFS
					$ImporterIndexToken = $ImporterIndexToken + $OFS
				} elseif ( ($PrevGuard -ne $null) -and ($NextGuard -ne $null) ) {
					# Inserted spacing
					# No padding needed for $HeaderIndexToken
					$SourceIndexToken = $SourceIndexToken + $OFS + $OFS
					$ImporterIndexToken = $ImporterIndexToken + $OFS
				} else {
					# Last guard in file!
					$HeaderIndexToken = $OFS + $HeaderIndexToken
					$SourceIndexToken = $SourceIndexToken + $OFS + $OFS
					$ImporterIndexToken = $OFS + $ImporterIndexToken + $OFS

				}

				foreach ($key in $FuncGuardFiles.keys) {
					$NewFileText = $null
					$ContentModified = $false
					[string]$FileText = Get-Content $FuncGuardFiles.item($key)

					switch ($key) {
						"ActiveHeader" {
							$NewFileText = $FileText.Remove(
								$FileText.IndexOf($HeaderIndexToken),
									$HeaderIndexToken.length)
							$ContentModified = $true
						}
						"GuardSource" {
							$NewFileText = $FileText.Remove(
								$FileText.IndexOf($SourceIndexToken),
									$SourceIndexToken.length)
							$ContentModified = $true
						}
						"GuardImport" {
							$NewFileText =$FileText.Remove(
								$FileText.IndexOf($ImporterIndexToken),
									$ImporterIndexToken.length)
							$ContentModified = $true
						}
					}

					if ($ContentModified) {
						Set-Content -Path $FuncGuardFiles.item($key) `
							-Value $NewFileText
					}
				}
				$FuncGuardRemoved = $true
			}
		}
		if (! ($FuncGuardRemoved) ) {
			$PSCmdlet.WriteError((
				New-ModuleError -ErrorCategory ObjectNotFound `
				-message "Cannot remove FuncGuard `'$Name`' because it does not exist." `
				-identifierText FuncGuardObjectNotFound -targetObject $($Name)
			))
		}
	}
}
function Update-FuncGuardFiles {
<#
	.SYNOPSIS
		Update Function Guard entries from the active header file to the skeleton
		and/or template files.
	.DESCRIPTION
		Update-FuncGuard reads the current function guard states from the active
		FuncGuards.h and inserts those into the FuncGuard_skel.h or template files
		while ensuring the enabled/disabled state of those func guards remain as
		they were previously set.  New guards that are added to template are added
		as disabled.  Function guards that are no longer valid are removed.
	.EXAMPLE
		PS C:\NPPCR_ROOT>Update-FuncGuard -path "$NPPCR_ROOT\PowerEditor\src\MISC\Debug\"
	.EXAMPLE
		PS C:\NPPCR_ROOT>Update-FuncGuard -path "$NPPCR_ROOT\PowerEditor\src\MISC\Debug\" `
		-templatePath "$NPPCR_ROOT\PowerEditor\src\MISC\Debug\templates"

	.PARAMETER Path
		Path to the function guard source files directory.
		Deafults to the active Function Guard project path.
	.PARAMETER TemplatePath
		Path to the function guard template files directory.
	.PARAMETER All
		Update both the skeleton and template files.
	.PARAMETER Skeleton
		Update only the skeleton header.
	.PARAMETER Template
		Update only the template header files.
#>
	[CmdletBinding(DefaultParameterSetName="Skeleton", SupportsShouldProcess=$true)]

	PARAM(
		[Parameter(ParameterSetName="All",Mandatory=$false,Position=0)]
		[Switch]$All
	,
		[Parameter(ParameterSetName="Skeleton",Mandatory=$false,Position=0)]
		[Switch]$Skeleton
	,
		[Parameter(ParameterSetName="Template",Mandatory=$false,Position=0)]
		[Switch]$Template
	,
		[Parameter(Position=1,Mandatory=$false,
			HelpMessage="`nPath skeleton header file.")]
		[ValidateScript({Test-Path -Path $_})]
		[string]
		$Path = $(Get-FuncGuardProjectPath)
	,
		[Parameter(ParameterSetName="Template",Position=1,Mandatory=$true,
			HelpMessage="`nPath to the template headers.")]
		[Parameter(ParameterSetName="All",Position=2,Mandatory=$false,
			HelpMessage="`nPath to the template headers.  Defaults to `$SourcePath\templates.")]
		[ValidateScript({Test-Path -Path $_ -PathType Container})]
		[string]$TemplatePath = $(if($All){"$Path\templates"})
	)
	Begin{
		$OFS = [Environment]::NewLine

		switch ($PSCmdlet.ParameterSetName) {
			"Skeleton" {
				$Skeleton = $true
				$Template = $false
				$SourceFiles = Get-FuncGuardCoreFiles $Path
			}
			"Template" {
				$Skeleton = $false
				$Template = $true
				$SourceFiles = Get-FuncGuardCoreFiles $Path
				$TemplateFiles = Get-FuncGuardTemplateFiles $TemplatePath
			}
			"All" {
				$Skeleton = $true
				$Template = $true
				$SourceFiles = Get-FuncGuardCoreFiles $Path
				$TemplateFiles = Get-FuncGuardTemplateFiles $TemplatePath
			}
		}

		try {
			if ($SourceFiles.item("ActiveHeader") -ne $Null) {
				$FuncGuard = @(Get-FuncGuard -Path $SourceFiles.item("ActiveHeader"))
			} else {
					throw (New-Object IO.FileNotFoundException $Path"\FuncGuards.h")
			}
		}
		catch {
			throw $_
		}
	}
	Process{
		$ActiveHeader = $SourceFiles.item("ActiveHeader")
		$TargetFile = @()
		if ( $Skeleton ) {
			$TargetFile += @($SourceFiles.item("SkeletonHeader"))
		}
		if ( $Template ) {
			$TargetFile += @($TemplateFiles.values)
		}

		$ActiveGuards = @(Get-FuncGuard -Path $ActiveHeader)
		foreach ($file in $TargetFile) {
			if ( $file -eq $SourceFiles.item("SkeletonHeader") ) {
				$fileGuards = @(Get-FuncGuard -Skeleton -Path $file -ea 0)
			} else {
				$fileGuards = @(Get-FuncGuard -Path $file -ea 0)
			}
			$tmpFile = "$($file).tmp"
			$doConfirm = $false
			Copy-Item -Path $ActiveHeader -Destination $tmpFile -Force
			Disable-FuncGuard -Path $tmpFile | Out-Null
			foreach ($guard in $fileGuards) {
				if ($ActiveGuards|select -Expand Name|?{$_ -eq $guard.name}) {
					if ($guard.Status -eq "Enabled") {
						Enable-FuncGuard -name $guard.Name -Path $tmpFile
					}
				} else {
					$msg = "Function guard `'$($guard.Name)`' will be removed from $($file)."
					$PSCmdlet.WriteWarning($msg)
				}
			}
			if ($PSCmdlet.ShouldProcess(
				$file, "Update function guard header file.")) {
				Copy-Item -Path $tmpFile -Destination $File -Force
			}
		}
	}
}
################################################################################
## Internal support functions.
################################################################################

function createHeaderContent {
	param([string] $Name, [string]$Prefix, [string]$Description)

$RawCodeBlock = @"
// func_guard(guard$Name);
// Description: $Description
//#define FUNCGUARD_$($Prefix.ToUpper())_$($Name.ToUpper())
"@

	return $RawCodeBlock
}
function createImporterContent {
	param([string] $Name)

$RawCodeBlock = @"
func_guard_import_cat(guard$Name);
"@

	return $RawCodeBlock
}
function createSourceContent {
	param([string] $Name, [string]$Prefix)

$RawCodeBlock = @"
#ifdef FUNCGUARD_$($Prefix.ToUpper())_$($Name.ToUpper())
        func_guard_enable_cat(guard$Name);
#else
        func_guard_disable_cat(guard$Name);
#endif
"@

	return $RawCodeBlock
}
function Get-FuncGuardProjectPrefix {
	return 	$FuncGuardSettings.("Configurations").(
		$($FuncGuardSettings.("ActiveConfiguration"))).Prefix
}
function Get-FuncGuardProjectPath {
	return 	$FuncGuardSettings.("Configurations").(
		$($FuncGuardSettings.("ActiveConfiguration"))).Path
}
function Get-FuncGuardSourceFiles {
	[CmdletBinding(DefaultParameterSetName="Source")]
	PARAM(
		[Parameter(ParameterSetName="All",Mandatory=$true,Position=0,
			HelpMessage="`nGet all required source files as well as template files.")]
		[Switch]$All
		,
		[Parameter(ParameterSetName="Source",Mandatory=$true,Position=0,
			HelpMessage="`nGet only the required header, source, and import header files.")]
		[Switch]$Source
		,
		[Parameter(ParameterSetName="Templates",Mandatory=$true,Position=0,
			HelpMessage="`nGet ony the template header files.")]
		[Switch]$Template
		,
		[Parameter(ParameterSetName="Source",Position=1,Mandatory=$true,
			HelpMessage="`nPath to the required header, source and import header files.")]
		[Parameter(ParameterSetName="All",Position=1,Mandatory=$true,
			HelpMessage="`nPath to the required header, source and import header files.")]
		[string]$SourcePath
		,
		[Parameter(ParameterSetName="Templates",Position=1,Mandatory=$true,
			HelpMessage="`nPath to the template headers.")]
		[Parameter(ParameterSetName="All",Position=2,Mandatory=$false,
			HelpMessage="`nPath to the template headers.  Defaults to `$SourcePath\templates.")]
		[string]$TemplatePath = $(if($All){"$SourcePath\templates"})
	)

	Process {
		$SourceFiles = @{}

		switch ($PSCmdlet.ParameterSetName)
		{
			"source" {
				$SourceFiles = Get-FuncGuardCoreFiles $SourcePath
			}
			"templates" {
				$SourceFiles = Get-FuncGuardTemplateFiles $TemplatePath
			}
			"all" {
				$SourceFiles = Get-FuncGuardCoreFiles $SourcePath
				$SourceFiles += Get-FuncGuardTemplateFiles $TemplatePath
			}
		}
	}

	End {
		return $SourceFiles
	}

}
function Get-FuncGuardCoreFiles {
	[CmdletBinding()]
	param([string]$Path)
	$RequiredSourceFiles = @{}

	$TestFiles = @{
		"ActiveHeader" = (Join-Path -Path $Path -ChildPath "FuncGuards.h");
		"SkeletonHeader" = (Join-Path -Path $Path -ChildPath "FuncGuards_skel.h");
		"GuardSource" = (Join-Path -Path $Path -ChildPath "FuncGuards.cpp");
		"GuardImport" = (Join-Path -Path $Path -ChildPath "FuncGuardsImport.h")
	}

	$TestFiles.keys | foreach {
		if (Test-Path -Path $TestFiles.$_ -PathType leaf) {
			$RequiredSourceFiles.add("$_", $TestFiles.$_ )
		} else {
			if ($_ -ne "ActiveHeader") {
				$PSCmdlet.WriteError((
					New-ModuleError -ErrorCategory ObjectNotFound `
					-message "Cannot get FuncGuard file `'$($TestFiles.$_)`' because it does not exist." `
					-identifierText FileNotFound -targetObject $($TestFiles.$_)
				))
			}
		}
	}

	return $RequiredSourceFiles
}
function Get-FuncGuardTemplateFiles {
	param([string]$Path)
	$TemplateFiles = @{}

	Get-ChildItem -Path $Path -ea 0 | Where-Object {$_.Name -match ".*_template.h"} | foreach {
		if (Test-Path -Path $_.PSPath -PathType leaf) {
			$TemplateFiles.add("$($_.Name)", $_.FullName )
		}
	}
	if ($TemplateFiles.count -eq 0) {
			throw (New-Object IO.FileNotFoundException "No template files found in $Path")
	}

	$TestFiles = $null

	return $TemplateFiles
}
function getHeaderPathFromPathParam {
	param([string]$Path)

	if ( Test-Path -Path $Path -PathType Container ) {
		$Path = Join-Path -Path $Path -ChildPath "FuncGuards.h"
		if (! (Test-Path -Path $Path -PathType Leaf ) ) {
			throw ( new-object IO.FileNotFoundException $Path )
		}
	}

	if (! ( Select-String -Path $Path `
		-Pattern "^\#define.*_DEBUG_FUNCGUARD_H" -List ) ) {
		Write-Error "Invalid file!  Skipping: $Path"
		continue
	}

	return $Path
}
function normalizeLineEnding {
	param([string]$String)
	if (! ($String -match [Environment]::NewLine)) {
		$String = $String.replace("`n", [Environment]::NewLine)
	}
	return $String
}
function testConfigurations {
	[CmdletBinding()]
	Param()

	$FoundActiveConfig=$true
	$FoundBuiltInConfig=$false
	$PrefixesValidated = $true
	$PathsValidated = $true

	if (! ( $FuncGuardSettings.containskey("ActiveConfiguration") -and
		$FuncGuardSettings.containskey("Configurations") -and
		$FuncGuardSettings.("Configurations").containskey(
			$FuncGuardSettings.("ActiveConfiguration") ) ) ) {

		$FoundActiveConfig = $false

	} else {
		$ConfigSet = $FuncGuardSettings.("Configurations")
		foreach ($ConfigKey in $ConfigSet.keys) {
			foreach ($Setting in $ConfigSet.item($ConfigKey).keys) {
				$value = $ConfigSet.item($ConfigKey).item($Setting)
				switch ($setting) {
					"Prefix" {
						try {
							testPrefixParam $value
						}
						catch {
							$PrefixesValidated = $false
							Write-Warning "Configuration $($ConfigKey) : $($_.Exception|out-string)"
						}
					}
					"Path" {
						try {
							testPathParam $value
						}
						catch {
							$PathsValidated = $false
							Write-Warning "The path value used in configuration $($ConfigKey) is not a valid path!"
						}
						$gfgcf_errors=$null
						Get-FuncGuardCoreFiles $value -ev gfgcf_errors
						if ($gfgcf_errors -ne $null) {
							$PathsValidated = $false
							Write-Warning "Required files are missing from configuration $($ConfigKey)"
						}
					}
					"Type" {
						if ($value -eq "Builtin") {
							$FoundBuiltInConfig = $true
						}
					}
				}
			}
		}
	}

	if (! ( $FoundActiveConfig -and $FoundBuiltInConfig -and
		$PrefixesValidated -and	$PathsValidated)) {

		$Module = $ExecutionContext.SessionState.Module
		$ModuleName = $Module.Name.Replace(".config",$null)

		$AppData = [System.Environment]::GetFolderPath(
			[System.Environment+SpecialFolder]::LocalApplicationData)
		$ModuleConfigPath = join-path -Path $AppData `
			-ChildPath "WindowsPowerShell\Modules\$ModuleName\$ModuleName.config.xml"

		$PSCmdlet.WriteError((
			New-ModuleError -ErrorCategory InvalidData `
			-message "Configuration file is corrupt!  Please manually correct or remove the configuration file.`n$($ModuleConfigPath)" `
			-identifierText ConfigurationFileError -targetObject $($ModuleConfigPath)
		))
	}
}
function testPrefixParam {
	param([string]$Prefix)

	if (! ($Prefix -cmatch "^\p{L}{3}[\w-[\p{P}]]{0,7}$") ) {
		throw (New-Object System.FormatException "Invalid function guard prefix format.")
	}
	return $true
}
function testPathParam {
	param([string]$Path)

	if (! (Test-Path -Path $Path -PathType Container) ) {
		throw (New-Object IO.DirectoryNotFoundException $Path);
	}

	return $true
}
function testNameParam {
	param([string]$Name)

	if (! ( $Name -cmatch "^(\p{Lu}\p{Ll}+)+($|_((\p{Lu}\p{Ll}+)|\d)+|\d+)+$" ) ) {
		throw (New-Object System.FormatException "Invalid function guard name format.")
	}

	return $true
}

################################################################################
## Import Actions
################################################################################
function initializeModule {
	[CmdletBinding()]
	param([bool]$PSunit)

	if ( $FuncGuardSettings.containskey("InitializeConfigurations") ) {
		$ConfigSet = $FuncGuardSettings.("Configurations")
		foreach ($ConfigKey in $ConfigSet.keys) {
			if (! (Test-Path $ConfigSet.$ConfigKey.Path) ) {
				New-FuncGuardProject -Force -Name $ConfigSet.$ConfigKey.Name `
					-Prefix $ConfigSet.$ConfigKey.Prefix `
					-Path $ConfigSet.$ConfigKey.Path
			}
		}
		testConfigurations -ErrorAction Continue
		$FuncGuardSettings.remove("InitializeConfigurations")
		$FuncGuardSettings.export()
	} else {
		testConfigurations -ErrorAction Continue
	}

	# Everything gets exported when running PSUnit tests
	if ( $PSUnit ) {
		Set-FuncGuardProject -Name "PSUnit"
	} else {
		Export-ModuleMember -Function `
			New-FuncGuardProject,
			Add-FuncGuardProject,
			Set-FuncGuardProject,
			Get-FuncGuardProject,
			Remove-FuncGuardProject,
			Add-FuncGuard,
			Get-FuncGuard,
			Enable-FuncGuard,
			Disable-FuncGuard,
			Remove-FuncGuard,
			Update-FuncGuardFiles
	}
}

if (! (Get-Module ModuleWriteError) ) {
	Import-Module "ModuleWriteError" -Global -ErrorAction Stop
}

$PSunit = $false
foreach ($arg in $args) {
	switch($arg){
		"PSUnit"{
			$PSunit=$true
		}
	}
}

if ( $ExecutionContext.SessionState.Module ) {
	initializeModule -PSUnit $PSunit -ErrorAction Stop
}
