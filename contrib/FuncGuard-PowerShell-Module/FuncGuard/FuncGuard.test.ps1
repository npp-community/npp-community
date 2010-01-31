################################################################################
## Script Name:     N++CR FuncGuard Module PSUnit Tests
## Created On:      12/19/2009
## Author:          Thell Fowler
## File:            nppcr-funcguard.test.ps1
## Usage:           PS> cd\path\to\nppcr-funcguards.psm1
##					PSUnit.Run.ps1 -PSUnitTestFile .\nppcr-funcguard.Test.ps1
## Purpose:         Execute unit testing for nppcr-funcguard.psm1
##
## Requirements:    PowerShell Version 2
##					PSUnit
##
################################################################################

<#
 Index:
	- Test Support Functions
	- testPathParam
	- testPrefixParam
	- testNameParam
	- createHeaderContent
	- createImporterContent
	- createSourceContent
	- New-FuncGuardProject
	- Add-FuncGuardProject
	- Get-FuncGuardProject
	- Set-FuncGuardProject
	- Remove-FuncGuardProject
	- Get-FuncGuardCoreFiles
	- Get-FuncGuardTemplateFiles
	- Get-FuncGuardSourceFiles
	- Add-FuncGuard
	- Get-FuncGuard
	- Enable-FuncGuard
	- Disable-FuncGuard
	- Remove-FuncGuard
	- Sync-FuncGuardFiles
#>

. PSUnit.ps1

$StarterRoot = $(Get-Variable -Name ScriptUnderTestRoot -Scope Global).Value
$TargetModule = join-path -Path $StarterRoot -ChildPath FuncGuard.psd1
$TMP_TESTFILES_PATH = join-path -Path $Env:TEMP -ChildPath "FuncGuard.PSUnit"
if (Test-Path $TMP_TESTFILES_PATH) {
	Remove-Item -Force -Recurse $TMP_TESTFILES_PATH\* -ea 0
} else {
	New-Item -Force $TMP_TESTFILES_PATH -ItemType Directory
}
$TargetFiles = @("FuncGuards.h", "FuncGuards_skel.h", "FuncGuards.cpp","FuncGuardsImport.h")
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
	$Source = Join-Path -Path $StarterRoot.ToString() -ChildPath $SourceName
	$Destination = Join-Path $TMP_TESTFILES_PATH -ChildPath $file
	Copy-Item  -Force -Path $Source -Destination $Destination
}

Import-Module $TargetModule -ArgumentList "PSUnit" -Force -ea 0 -ev im_error
if ($im_error) {
	Write-Warning $($im_error|Out-String)
	throw (New-Object System.InvalidOperationException `
		"Unit testing can not be run because of Module errors!"
	)
}

################################################################################
##  - Support Functions
################################################################################

function Create-TmpFuncGuardTestFileContainer {
	# Create temporary path for required files
	if (! (Test-Path -Path $TMP_TESTFILES_PATH -PathType Container) ) {
		New-Item -Path $TMP_TESTFILES_PATH -ItemType Directory
	}
}
function Prepare-RequiredFilesPath {
	# Ensure required files are available
	Create-TmpFuncGuardTestFileContainer
	$requiredfiles=(
		("FuncGuards_skel.h", "FuncGuardsImport.h", "FuncGuards.cpp") |
		foreach {Join-Path -Path $TMP_TESTFILES_PATH -ChildPath $_} )
	$requiredfiles | foreach {New-Item -Force -Path $_ -ItemType file}
	$requiredfiles | foreach { "TestFile" | Out-File $_ }
}
function Test-RequiredFilesAvailable {
	# Makes sure the required files are present
	$prep=$false
	$requiredfiles=@(
		"FuncGuards_skel.h", "FuncGuardsImport.h", "FuncGuards.cpp")
	$requiredfiles | foreach {
		if (!( Test-Path -Path $TMP_TESTFILES_PATH\$_ -PathType Leaf)) {
			$prep=$true
		}
	}
	if ($prep) {
		Prepare-RequiredFilesPath
	}
	return $true
}
function Clear-TmpFuncTestDirectory {
	# Ensure required files are cleaned up.
	get-childitem $TMP_TESTFILES_PATH\* | `
		Where-Object {$_.psIsContainer -eq $false} | Remove-Item -Force
}
function Delete-TmpFuncGuardTestFile-Container {
	# Remove temporary path for required files
	Remove-Item -Path $TMP_TESTFILES_PATH -Force
}

function Prepare-OptionalFilesPath {
	# Ensure required files are available
	Create-TmpFuncGuardTestFileContainer
	if (!(Test-Path -Path $TMP_TESTFILES_PATH\templates -PathType Container)) {
		New-Item -Path $TMP_TESTFILES_PATH\templates -ItemType Directory
	}
	$requiredfiles=( ("FuncGuards1_template.h", "FuncGuards2_template.h", `
		"FuncGuards3_template.h") |
		foreach {Join-Path -Path $TMP_TESTFILES_PATH\templates -ChildPath $_} )
	$requiredfiles | foreach {New-Item -Force -Path $_ -ItemType file}
	$requiredfiles | foreach { "TestFile" | Out-File $_ }
}
function Test-OptionalFilesAvailable {
	# Makes sure the optional files are present
	if ( @(Get-ChildItem -Path "$TMP_TESTFILES_PATH\templates\*.template.h" `
		-ErrorAction SilentlyContinue).Count -ne 3 ) {
		Prepare-OptionalFilesPath
	}
	return $true
}
function Clear-TmpFuncTestOptionalDirectory {
	# Ensure required files are cleaned up.
	Remove-Item -Path $TMP_TESTFILES_PATH\templates\* -Force -ea 0
}
function Delete-TmpFuncGuardTestOptionalFile-Container {
	# Remove temporary path for required files
	Remove-Item -Path $TMP_TESTFILES_PATH\templates -Force -ea 0
}

## Here-strings
$headerContent_Empty = @'
/*
	Simulate
	License
	Block
*/

/*
	Simulate
	User
	Instruction
	Block
*/

#ifndef MISC_DEBUG_FUNCGUARD_H
#define MISC_DEBUG_FUNCGUARD_H

#ifndef SHIPPING

#endif // SHIPPING
#endif // MISC_DEBUG_FUNCGUARD_H
'@
$headerContent_Single = @'
/*
	Simulate
	License
	Block
*/

/*
	Simulate
	User
	Instruction
	Block
*/

#ifndef MISC_DEBUG_FUNCGUARD_H
#define MISC_DEBUG_FUNCGUARD_H

#ifndef SHIPPING

// func_guard(guardClass1);
// Description: Used in tracing functions in the Class1 class.
// #define FUNCGUARD_PSUNIT_CLASS1

#endif // SHIPPING
#endif // MISC_DEBUG_FUNCGUARD_H
'@
$headerContent_SingleEnabled = @'
/*
	Simulate
	License
	Block
*/

/*
	Simulate
	User
	Instruction
	Block
*/

#ifndef MISC_DEBUG_FUNCGUARD_H
#define MISC_DEBUG_FUNCGUARD_H

#ifndef SHIPPING

// func_guard(guardClass1);
// Description: Used in tracing functions in the Class1 class.
#define FUNCGUARD_PSUNIT_CLASS1

#endif // SHIPPING
#endif // MISC_DEBUG_FUNCGUARD_H
'@
$headerContent_ThreeValidAllDisabled = @'
/*
	Simulate
	License
	Block
*/

/*
	Simulate
	User
	Instruction
	Block
*/

#ifndef MISC_DEBUG_FUNCGUARD_H
#define MISC_DEBUG_FUNCGUARD_H

#ifndef SHIPPING

// func_guard(guardClass1);
// Description: Used in tracing functions in the Class1 class.
//#define FUNCGUARD_PSUNIT_CLASS1

// func_guard(guardClass1Class2_Communications);
// Description: For tracing TestGuard1 and TestGuard2 communication.
/*
	This enables tracing of _only_ the communication functions of the
	TestGuard1 and TestGuard2 classes.

	Modify the local function area to filter messages, to output use:
		func_guard_debugf_cat(guardTestGuard_MultiClass);

	Enable by adding to the current func_guard(#cat) statement.
		example: func_guard(guardClass1 | guardClass1Class2_Communications);

	If both TestGuard1 and TestGuard2 are active then enabling this is not
	needed.
*/
//#define FUNCGUARD_PSUNIT_CLASS1CLASS2_COMMUNICATIONS

// func_guard(guardClass2);
// Description: Used in tracing functions in the Class2 class.
//#define FUNCGUARD_PSUNIT_CLASS2

#endif // SHIPPING
#endif // MISC_DEBUG_FUNCGUARD_H
'@
$headerContent_ThreeValidSingleEnabled = @'
/*
	Simulate
	License
	Block
*/

/*
	Simulate
	User
	Instruction
	Block
*/

#ifndef MISC_DEBUG_FUNCGUARD_H
#define MISC_DEBUG_FUNCGUARD_H

#ifndef SHIPPING

// func_guard(guardClass1);
// Description: Used in tracing functions in the Class1 class.
#define FUNCGUARD_PSUNIT_CLASS1

// func_guard(guardClass1Class2_Communications);
// Description: For tracing TestGuard1 and TestGuard2 communication.
/*
	This enables tracing of _only_ the communication functions of the
	TestGuard1 and TestGuard2 classes.

	Modify the local function area to filter messages, to output use:
		func_guard_debugf_cat(guardTestGuard_MultiClass);

	Enable by adding to the current func_guard(#cat) statement.
		example: func_guard(guardClass1 | guardClass1Class2_Communications);

	If both TestGuard1 and TestGuard2 are active then enabling this is not
	needed.
*/
//#define FUNCGUARD_PSUNIT_CLASS1CLASS2_COMMUNICATIONS

// func_guard(guardClass2);
// Description: Used in tracing functions in the Class2 class.
//#define FUNCGUARD_PSUNIT_CLASS2

#endif // SHIPPING
#endif // MISC_DEBUG_FUNCGUARD_H
'@
$headerContent_ThreeValidAllEnabled = @'
/*
	Simulate
	License
	Block
*/

/*
	Simulate
	User
	Instruction
	Block
*/

#ifndef MISC_DEBUG_FUNCGUARD_H
#define MISC_DEBUG_FUNCGUARD_H

#ifndef SHIPPING

// func_guard(guardClass1);
// Description: Used in tracing functions in the Class1 class.
#define FUNCGUARD_PSUNIT_CLASS1

// func_guard(guardClass1Class2_Communications);
// Description: For tracing TestGuard1 and TestGuard2 communication.
/*
	This enables tracing of _only_ the communication functions of the
	TestGuard1 and TestGuard2 classes.

	Modify the local function area to filter messages, to output use:
		func_guard_debugf_cat(guardTestGuard_MultiClass);

	Enable by adding to the current func_guard(#cat) statement.
		example: func_guard(guardClass1 | guardClass1Class2_Communications);

	If both TestGuard1 and TestGuard2 are active then enabling this is not
	needed.
*/
#define FUNCGUARD_PSUNIT_CLASS1CLASS2_COMMUNICATIONS

// func_guard(guardClass2);
// Description: Used in tracing functions in the Class2 class.
#define FUNCGUARD_PSUNIT_CLASS2

#endif // SHIPPING
#endif // MISC_DEBUG_FUNCGUARD_H
'@
$headerContent_FourValidAllEnabled = @'
/*
	Simulate
	License
	Block
*/

/*
	Simulate
	User
	Instruction
	Block
*/

#ifndef MISC_DEBUG_FUNCGUARD_H
#define MISC_DEBUG_FUNCGUARD_H

#ifndef SHIPPING

// func_guard(guardClass0);
// Description: Used in tracing functions in the Class0 class.
#define FUNCGUARD_PSUNIT_CLASS0

// func_guard(guardClass1);
// Description: Used in tracing functions in the Class1 class.
#define FUNCGUARD_PSUNIT_CLASS1

// func_guard(guardClass1Class2_Communications);
// Description: For tracing TestGuard1 and TestGuard2 communication.
/*
	This enables tracing of _only_ the communication functions of the
	TestGuard1 and TestGuard2 classes.

	Modify the local function area to filter messages, to output use:
		func_guard_debugf_cat(guardTestGuard_MultiClass);

	Enable by adding to the current func_guard(#cat) statement.
		example: func_guard(guardClass1 | guardClass1Class2_Communications);

	If both TestGuard1 and TestGuard2 are active then enabling this is not
	needed.
*/
#define FUNCGUARD_PSUNIT_CLASS1CLASS2_COMMUNICATIONS

// func_guard(guardClass2);
// Description: Used in tracing functions in the Class2 class.
#define FUNCGUARD_PSUNIT_CLASS2

#endif // SHIPPING
#endif // MISC_DEBUG_FUNCGUARD_H
'@
$SourceContent_Empty = @"
/*
	Simulate
	License
	Block
*/

/*
	Simulate
	User
	Instruction
	Block
*/

// FUNCGUARD_BLOCK_START << Do not alter or remove

// FUNCGUARD_BLOCK_END << Do not alter or remove
"@
$SourceContent_Single = @"
/*
	Simulate
	License
	Block
*/

/*
	Simulate
	User
	Instruction
	Block
*/

// FUNCGUARD_BLOCK_START << Do not alter or remove

#ifdef FUNCGUARD_PSUNIT_CLASS1
        func_guard_enable_cat(guardClass1);
#else
        func_guard_disable_cat(guardClass1);
#endif

// FUNCGUARD_BLOCK_END << Do not alter or remove
"@
$SourceContent = @"
/*
	Simulate
	License
	Block
*/

/*
	Simulate
	User
	Instruction
	Block
*/

// FUNCGUARD_BLOCK_START << Do not alter or remove

#ifdef FUNCGUARD_PSUNIT_CLASS1
        func_guard_enable_cat(guardClass1);
#else
        func_guard_disable_cat(guardClass1);
#endif

#ifdef FUNCGUARD_PSUNIT_CLASS1CLASS2_COMMUNICATIONS
        func_guard_enable_cat(guardClass1Class2_Communications);
#else
        func_guard_disable_cat(guardClass1Class2_Communications);
#endif

#ifdef FUNCGUARD_PSUNIT_CLASS2
        func_guard_enable_cat(guardClass2);
#else
        func_guard_disable_cat(guardClass2);
#endif

// FUNCGUARD_BLOCK_END << Do not alter or remove
"@
$ImporterContent_Empty = @"
/*
	Simulate
	License
	Block
*/

/*
	Simulate
	User
	Instruction
	Block
*/

#ifndef MISC_DEBUG_FUNCGUARDIMPORT_H
#define MISC_DEBUG_FUNCGUARDIMPORT_H

#endif // MISC_DEBUG_FUNCGUARDS_H
"@
$ImporterContent_Single = @"
/*
	Simulate
	License
	Block
*/

/*
	Simulate
	User
	Instruction
	Block
*/

#ifndef MISC_DEBUG_FUNCGUARDIMPORT_H
#define MISC_DEBUG_FUNCGUARDIMPORT_H

func_guard_import_cat(guardClass1);

#endif // MISC_DEBUG_FUNCGUARDS_H
"@
$ImporterContent = @"
/*
	Simulate
	License
	Block
*/

/*
	Simulate
	User
	Instruction
	Block
*/

#ifndef MISC_DEBUG_FUNCGUARDIMPORT_H
#define MISC_DEBUG_FUNCGUARDIMPORT_H

func_guard_import_cat(guardClass1);
func_guard_import_cat(guardClass1Class2_Communications);
func_guard_import_cat(guardClass2);

#endif // MISC_DEBUG_FUNCGUARDS_H
"@
$ExpectedRawCodeBlock_SimpleDescription = @"
// func_guard(guardClass1);
// Description: Used in tracing functions in the Class1 class.
//#define FUNCGUARD_PSUNIT_CLASS1
"@
$ExpectedRawCodeBlock_InstreamDescription = @"
// func_guard(guardClass1);
// Description: Used in tracing functions in the Class1 class.
/*
	An instream description
*/
//#define FUNCGUARD_PSUNIT_CLASS1
"@
$ExpectedRawCodeBlock_Importer = @"
func_guard_import_cat(guardClass1);
"@
$ExpectedRawCodeBlock_Source = @"
#ifdef FUNCGUARD_PSUNIT_CLASS1
        func_guard_enable_cat(guardClass1);
#else
        func_guard_disable_cat(guardClass1);
#endif
"@

################################################################################
##  - testPathParam
################################################################################
function Test.testPathParam_InvalidPath(
	[switch] $Category_ParameterValidation,
	[System.IO.DirectoryNotFoundException] $ExpectedException = $Null) {
    #Arrange
	$Path = "C:\AnInvalidPath"

	#Act
    $Actual = (testPathParam -Path "$Path")

	#Assert
}
function Test.testPathParam_ValidPath(
	[switch] $Category_ParameterValidation) {
    #Arrange
	$Path = "C:\"

	#Act
    $Actual = (testPathParam -Path "$Path")

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq $true}
}

################################################################################
##  - testPrefixParam
################################################################################
function Test.testPrefixParam_InValid_ToShort__PR(
	[switch] $Category_ParameterValidation,
	[System.FormatException] $ExpectedException = $null) {
	#Arrange
	$Prefix = "PR"

	#Act
	$Actual = testPrefixParam $Prefix

	#Assert
}
function Test.testPrefixParam_InValid_ToLong__ALONGPREFIX(
	[switch] $Category_ParameterValidation,
	[System.FormatException] $ExpectedException = $null) {
	#Arrange
	$Prefix = "ALONGPREFIX"

	#Act
	$Actual = testPrefixParam $Prefix

	#Assert
}
function Test.testPrefixParam_InValid_Chars__PRE_FIX(
	[switch] $Category_ParameterValidation,
	[System.FormatException] $ExpectedException = $null) {
	#Arrange
	$Prefix = "PRE_FIX"

	#Act
	$Actual = testPrefixParam $Prefix

	#Assert
}
function Test.testPrefixParam_InValid_LeadingDigit__1PREFIX(
	[switch] $Category_ParameterValidation,
	[System.FormatException] $ExpectedException = $null) {
	#Arrange
	$Prefix = "1PREFIX"

	#Act
	$Actual = testPrefixParam $Prefix

	#Assert
}
function Test.testPrefixParam_InValid_NotEnoughLettersBeforeDigit__PR01(
	[switch] $Category_ParameterValidation,
	[System.FormatException] $ExpectedException = $null) {
	#Arrange
	$Prefix = "PR01"

	#Act
	$Actual = testPrefixParam $Prefix

	#Assert
}
function Test.testPrefixParam_Valid_PREFIX(
	[switch] $Category_ParameterValidation) {
	#Arrange
	$Prefix = "Prefix"

	#Act
	$Actual = testPrefixParam $Prefix

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq $true}
}
function Test.testPrefixParam_Valid_Lower_Prefix(
	[switch] $Category_ParameterValidation) {
	#Arrange
	$Prefix = "Prefix"

	#Act
	$Actual = testPrefixParam $Prefix

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq $true}
}
function Test.testPrefixParam_Valid_WithDigit_PRE01(
	[switch] $Category_ParameterValidation) {
	#Arrange
	$Prefix = "PRE01"

	#Act
	$Actual = testPrefixParam $Prefix

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq $true}
}
function Test.testPrefixParam_Valid_WithDigit_PRE01FIX02(
	[switch] $Category_ParameterValidation) {
	#Arrange
	$Prefix = "PRE01FIX02"

	#Act
	$Actual = testPrefixParam $Prefix

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq $true}
}

################################################################################
##  - testNameParam
################################################################################
function Test.testNameParam_Valid__FuncName(
	[switch] $Category_ParameterValidation) {
    #Arrange
	$Name = "FuncName"

	#Act
    $Actual = (testNameParam -name "$Name")

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq $true}
}
function Test.testNameParam_Valid__FuncName_Part(
	[switch] $Category_ParameterValidation) {
    #Arrange
	$Name = "FuncName_Part"

	#Act
    $Actual = (testNameParam -Name "$Name")

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq $true}
}
function Test.testNameParam_Valid__FuncName_NamePart(
	[switch] $Category_ParameterValidation) {
    #Arrange
	$Name = "FuncName_NamePart"

	#Act
    $Actual = (testNameParam -Name "$Name")

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq $true}
}
function Test.testNameParam_Valid__FuncName_NamePart_NamePart(
	[switch] $Category_ParameterValidation) {
    #Arrange
	$Name = "FuncName_NamePart_NamePart"

	#Act
    $Actual = (testNameParam -Name "$Name")

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq $true}
}
function Test.testNameParam_Valid__FuncName1_NamePart_NamePart(
	[switch] $Category_ParameterValidation) {
    #Arrange
	$Name = "FuncName1_NamePart_NamePart"

	#Act
    $Actual = (testNameParam -Name "$Name")

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq $true}
}
function Test.testNameParam_Valid__FuncName1_1NamePart_NamePart(
	[switch] $Category_ParameterValidation) {
    #Arrange
	$Name = "FuncName1_1NamePart_NamePart"

	#Act
    $Actual = (testNameParam -Name "$Name")

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq $true}
}
function Test.testNameParam_Valid__FuncName_1NamePart_NamePart(
	[switch] $Category_ParameterValidation) {
    #Arrange
	$Name = "FuncName_1NamePart_NamePart"

	#Act
    $Actual = (testNameParam -Name "$Name")

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq $true}
}
function Test.testNameParam_Valid__FuncName_NamePart1_NamePart(
	[switch] $Category_ParameterValidation) {
    #Arrange
	$Name = "FuncName_NamePart1_NamePart"

	#Act
    $Actual = (testNameParam -Name "$Name")

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq $true}
}
function Test.testNameParam_Valid__FuncName_NamePart1_1NamePart(
	[switch] $Category_ParameterValidation) {
    #Arrange
	$Name = "FuncName_NamePart1_1NamePart"

	#Act
    $Actual = (testNameParam -Name "$Name")

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq $true}
}
function Test.testNameParam_Valid__FuncName_NamePart_1NamePart(
	[switch] $Category_ParameterValidation) {
    #Arrange
	$Name = "FuncName_NamePart_1NamePart"

	#Act
    $Actual = (testNameParam -Name "$Name")

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq $true}
}
function Test.testNameParam_Valid__FuncName_NamePart_NamePart1(
	[switch] $Category_ParameterValidation) {
    #Arrange
	$Name = "FuncName_NamePart_NamePart1"

	#Act
    $Actual = (testNameParam -Name "$Name")

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq $true}
}
function Test.testNameParam_Valid__FuncName_NamePart_1_NamePart(
	[switch] $Category_ParameterValidation) {
    #Arrange
	$Name = "FuncName_NamePart_1_NamePart"

	#Act
    $Actual = (testNameParam -Name "$Name")

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq $true}
}
function Test.testNameParam_Valid__FuncName_NamePart_NamePart_1(
	[switch] $Category_ParameterValidation) {
    #Arrange
	$Name = "FuncName_NamePart_NamePart_1"

	#Act
    $Actual = (testNameParam -Name "$Name")

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq $true}
}
function Test.testNameParam_InValidUpper__FUNCNAME(
	[switch] $Category_ParameterValidation,
	[System.FormatException] $ExpectedException = $null) {
    #Arrange
	$Name = "FUNCNAME"

	#Act & Assert
	$Actual = testNameParam -Name $Name

	#Assert
}
function Test.testNameParam_InValidLower__funcname(
	[switch] $Category_ParameterValidation,
	[System.FormatException] $ExpectedException = $null) {
    #Arrange
	$Name = "funcname"

	#Act & Assert
	$Actual = testNameParam -Name $Name

	#Assert
}
function Test.testNameParam_InValidUpper__Func_NAME(
	[switch] $Category_ParameterValidation,
	[System.FormatException] $ExpectedException = $null) {
    #Arrange
	$Name = "Func_NAME"

	#Act & Assert
	$Actual = testNameParam -Name $Name

	#Assert
}
function Test.testNameParam_InValidPartialUpper__FUnc_Name(
	[switch] $Category_ParameterValidation,
	[System.FormatException] $ExpectedException = $null) {
    #Arrange
	$Name = "FUnc_Name"

	#Act & Assert
	$Actual = testNameParam -Name $Name

	#Assert
}
function Test.testNameParam_InValidPartialUpper2__Func_NAme(
	[switch] $Category_ParameterValidation,
	[System.FormatException] $ExpectedException = $null) {
    #Arrange
	$Name = "Func_NAme"

	#Act & Assert
	$Actual = testNameParam -Name $Name

	#Assert
}
function Test.testNameParam_InValidPartialLower__fuNc_Name(
	[switch] $Category_ParameterValidation,
	[System.FormatException] $ExpectedException = $null) {
    #Arrange
	$Name = "fuNc_Name"

	#Act & Assert
	$Actual = testNameParam -Name $Name

	#Assert
}
function Test.testNameParam_InValidLower__Func_name(
	[switch] $Category_ParameterValidation,
	[System.FormatException] $ExpectedException = $null) {
    #Arrange
	$Name = "Func_name"

	#Act & Assert
	$Actual = testNameParam -Name $Name

	#Assert
}
function Test.testNameParam_InValidSpacing__Func__NAME(
	[switch] $Category_ParameterValidation,
	[System.FormatException] $ExpectedException = $null) {
    #Arrange
	$Name = "Func__Name"

	#Act & Assert
	$Actual = testNameParam -Name $Name

	#Assert
}
function Test.testNameParam_InValidSpacing__Func_NAME_(
	[switch] $Category_ParameterValidation,
	[System.FormatException] $ExpectedException = $null) {
    #Arrange
	$Name = "Func__Name_"

	#Act & Assert
	$Actual = testNameParam -Name $Name

	#Assert
}
function Test.testNameParam_InValidLeading__1Func_Name(
	[switch] $Category_ParameterValidation,
	[System.FormatException] $ExpectedException = $null) {
    #Arrange
	$Name = "1Func_Name"

	#Act & Assert
	$Actual = testNameParam -Name $Name

	#Assert
}
function Test.testNameParam_InValidPunctation__Func_NamePercent(
	[switch] $Category_ParameterValidation,
	[System.FormatException] $ExpectedException = $null) {
    #Arrange
	$Name = "Func_Name%"

	#Act & Assert
	$Actual = testNameParam -Name $Name

	#Assert
}

################################################################################
##  - createHeaderContent
################################################################################
function Test.createHeaderContent_SimpleDescription(
	[switch] $Category_Internal) {
	#Arrange
	$Prefix = "PSUnit"
	$Name = "Class1"
	$Description = "Used in tracing functions in the Class1 class."

	#Act
	$Actual = createHeaderContent -prefix $Prefix -Name $Name -Description $Description

	#Assert
	Assert-That -ActualValue $Actual -Constraint {
		$Actual -eq $ExpectedRawCodeBlock_SimpleDescription}
}
function Test.createHeaderContent_InStreamDescription_SingleLineString(
	[switch] $Category_Internal) {
	#Arrange
	$Prefix = "PSUnit"
	$Name = "Class1"
	$Description = "Used in tracing functions in the Class1 class."
	$Description +=	"`r`n/*`r`n`tAn instream description`r`n*/"

	#Act
	$Actual = createHeaderContent -prefix $Prefix -Name $Name `
		-Description $Description

	#Assert
	Assert-That -ActualValue $Actual -Constraint {
		$Actual -eq $ExpectedRawCodeBlock_InstreamDescription}
}
function Test.createHeaderContent_InStreamDescription_HereString(
	[switch] $Category_Internal) {
	#Arrange
	$Prefix = "PSUnit"
	$Name = "Class1"
$InstreamPart = @"
/*
	An instream description
*/
"@
	$Description = "Used in tracing functions in the Class1 class."
	$Description += "`r`n" + $InstreamPart

	#Act
	$Actual = createHeaderContent -prefix $Prefix -Name $Name `
		-Description $Description

	#Assert
	Assert-That -ActualValue $Actual -Constraint {
		$Actual -eq $ExpectedRawCodeBlock_InstreamDescription}
}

################################################################################
##  - createImporterContent
################################################################################
function Test.createImporterContent_Simple(
	[switch] $Category_Internal) {
	#Arrange
	$Name = "Class1"

	#Act
	$Actual = createImporterContent -Name $Name

	#Assert
	Assert-That -ActualValue $Actual -Constraint {
		$Actual -eq $ExpectedRawCodeBlock_Importer}
}

################################################################################
##  - createSourceContent
################################################################################
function Test.createSourceContent_Simple(
	[switch] $Category_Internal) {
	#Arrange
	$Name = "Class1"
	$Prefix = "PSUnit"

	#Act
	$Actual = createSourceContent -Name $Name -Prefix $Prefix

	#Assert
	Assert-That -ActualValue $Actual -Constraint {
		$Actual -eq $ExpectedRawCodeBlock_Source}
}

################################################################################
##  - New-FuncGuardProject
################################################################################
function Test.New-FuncGuardProject_Invalid_Prefix(
	[switch] $Category_Exported,
	[System.Management.Automation.ParameterBindingException] $ExpectedException `
	= "Actual: System.Management.Automation.ParameterBindingValidationException") {
    #Arrange

	#Act
    $Actual = New-FuncGuardProject -Name PSUnit2 -Prefix PR `
		-Path $TMP_TESTFILES_PATH

	#Assert
}
function Test.New-FuncGuardProject_Invalid_DuplicateName(
	[switch] $Category_Exported,
	[System.Management.Automation.ParameterBindingException] $ExpectedException `
	= "Actual: System.Management.Automation.ParameterBindingValidationException") {
    #Arrange

	#Act
    $Actual = New-FuncGuardProject -Name PSUnit -Prefix PSUNIT `
		-Path $TMP_TESTFILES_PATH

	#Assert
}
function Test.New-FuncGuardProject_Invalid_FilesAlreadyExist(
	[switch] $Category_Exported,
	[System.ApplicationException] $ExpectedException = `
	"Actual: FuncGuardModuleException") {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable

	#Act
    $Actual = New-FuncGuardProject -Name PSUFile -Prefix PSUFILE `
		-Path $TMP_TESTFILES_PATH

	#Assert
}
function Test.New-FuncGuardProject_Invalid_FilesSomeAlreadyExist(
	[switch] $Category_Exported,
	[System.ApplicationException] $ExpectedException = `
	"Actual: FuncGuardModuleException") {
    #Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	Remove-Item -Force "$TMP_TESTFILES_PATH\FuncGuards_skel.h"

	#Act
    $Actual = New-FuncGuardProject -Name PSUFILE -Prefix PSUFILE `
		-Path $TMP_TESTFILES_PATH

	#Assert
}
function Test.New-FuncGuardProject_Valid_NonExistingPath(
	[switch] $Category_Exported) {
    #Arrange
	$Path = Join-Path -Path $(Split-Path $TMP_TESTFILES_PATH -Parent) `
		-ChildPath "FuncGuard.PSUnit.NEWTESTDIR"
	if (Test-Path $Path) {
		Remove-Item -Force -Recurse $Path\*
		Remove-Item -Force -Recurse $Path
	}

	#Act
    New-FuncGuardProject -Name "PSNEP" -Prefix "PSNEP" -Path $Path
	$Actual = (
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuards.h") -PathType Leaf) -and
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuards_skel.h") -PathType Leaf) -and
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuards.cpp") -PathType Leaf) -and
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuardsImport.h") -PathType Leaf)
	)
	Remove-Item -Force -Recurse $Path

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq $true}
}
function Test.New-FuncGuardProject_Valid_NonExistingNestedPath(
	[switch] $Category_Exported) {
    #Arrange
	$Path = Join-Path -Path $(Split-Path $TMP_TESTFILES_PATH -Parent) `
		-ChildPath "FuncGuard.PSUnit.NEWTESTDIR\Nested\Deeply"
	if (Test-Path $Path) {
		Remove-Item -Force -Recurse $Path\*
		Remove-Item -Force -Recurse $Path
	}

	#Act
    New-FuncGuardProject -Name "PSNEP2" -Prefix "PSNEP2" -Path $Path
    $Actual = (
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuards.h") -PathType Leaf) -and
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuards_skel.h") -PathType Leaf) -and
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuards.cpp") -PathType Leaf) -and
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuardsImport.h") -PathType Leaf)
	)
	Remove-Item -Force -Recurse $Path

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq $true}
}
function Test.New-FuncGuardProject_Valid_ForceNameExists(
	[switch] $Category_Exported) {
    #Arrange
	$Path = Join-Path -Path $(Split-Path $TMP_TESTFILES_PATH -Parent) `
		-ChildPath "FuncGuard.PSUnit.NEWTESTDIR"
	if (Test-Path $Path) {
		Remove-Item -Force -Recurse $Path\*
		Remove-Item -Force -Recurse $Path
	}

	#Act
	$Configuration=@{"Name"="PSUnitNewForce";"Path"=$Path;"Prefix"="PSUNIT";"Type"="Custom"}
	$FuncGuardSettings.("Configurations").add("PSUnitNewForce",$Configuration)
    New-FuncGuardProject -Force -Name "PSNEP2" -Prefix "PSNEP2" -Path $Path
    $Actual = (
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuards.h") -PathType Leaf) -and
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuards_skel.h") -PathType Leaf) -and
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuards.cpp") -PathType Leaf) -and
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuardsImport.h") -PathType Leaf)
	)
	Remove-Item -Force -Recurse $Path

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq $true}
}
################################################################################
##  - Add-FuncGuardProject
################################################################################
function Test.Add-FuncGuardProject_Invalid_FileAsPath(
	[switch] $Category_Exported,
	[System.Management.Automation.ParameterBindingException] $ExpectedException `
	= "Actual: System.Management.Automation.ParameterBindingValidationException") {
    #Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	Copy-Item $TMP_TESTFILES_PATH\FuncGuards_skel.h $TMP_TESTFILES_PATH\FuncGuards.h

	#Act
    $Actual = Add-FuncGuardProject -Name PSUnit2 -Prefix PSUNIT `
		-Path $TMP_TESTFILES_PATH\FuncGuards.h

	#Assert
}
function Test.Add-FuncGuardProject_Invalid_FilesSomeDoNotExist(
	[switch] $Category_Exported,
	[System.ApplicationException] $ExpectedException = `
	"Actual: FuncGuardModuleException") {
    #Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable

	#Act
    $Actual = Add-FuncGuardProject -Name PSUnitAdd -Prefix PSUFILE `
		-Path $TMP_TESTFILES_PATH

	#Assert
}
function Test.Add-FuncGuardProject_Invalid_Prefix(
	[switch] $Category_Exported,
	[System.Management.Automation.ParameterBindingException] $ExpectedException `
	= "Actual: System.Management.Automation.ParameterBindingValidationException") {
    #Arrange

	#Act
    Add-FuncGuardProject -Name PSUnit2 -Prefix PR `
		-Path $TMP_TESTFILES_PATH

	#Assert
}
function Test.Add-FuncGuardProject_Invalid_DuplicateName(
	[switch] $Category_Exported,
	[System.Management.Automation.ParameterBindingException] $ExpectedException `
	= "Actual: System.Management.Automation.ParameterBindingValidationException") {
    #Arrange

	#Act
    $Actual = Add-FuncGuardProject -Name PSUnit -Prefix PSUNIT `
		-Path $TMP_TESTFILES_PATH

	#Assert
}
function Test.Add-FuncGuardProject_Valid_FilesExist(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$Path = $TMP_TESTFILES_PATH
	Copy-Item $Path\FuncGuards_skel.h $Path\FuncGuards.h

	#Act
	Add-FuncGuardProject -Name "PSUnitAdd1" -Prefix "PSUNIT" -Path $Path
    $Actual = ( ($FuncGuardSettings.("Configurations").containskey("PSUnitAdd1")) -and
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuards.h") -PathType Leaf) -and
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuards_skel.h") -PathType Leaf) -and
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuards.cpp") -PathType Leaf) -and
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuardsImport.h") -PathType Leaf)
	)

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq $true}
}
function Test.Add-FuncGuardProject_Valid_FilesDoNotExist(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	$Path = $TMP_TESTFILES_PATH

	#Act
	Add-FuncGuardProject -Name "PSUnitAdd2" -Prefix "PSUNIT" -Path $Path
    $Actual = ( ($FuncGuardSettings.("Configurations").containskey("PSUnitAdd2")) -and
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuards.h") -PathType Leaf) -and
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuards_skel.h") -PathType Leaf) -and
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuards.cpp") -PathType Leaf) -and
		(test-path (Join-Path -Path $Path -ChildPath "FuncGuardsImport.h") -PathType Leaf)
	)

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq $true}
}

################################################################################
##  - Get-FuncGuardProject
################################################################################
function Test.Get-FuncGuardProject_NonExistingProject(
	[switch] $Category_Exported) {
    #Arrange

	#Act
    $Actual = ( $(Get-FuncGuardProject -Name NonExisting) -eq $null )

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq $true}
}
function Test.Get-FuncGuardProject_AllProjects(
	[switch] $Category_Exported) {
    #Arrange
	$configCount = ($FuncGuardSettings.("Configurations").keys).count
	#Act
    $Actual = ( $(Get-FuncGuardProject).count -eq $configCount )

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq $true}
}
function Test.Get-FuncGuardProject_AllProjects(
	[switch] $Category_Exported) {
    #Arrange
	$configCount = ($FuncGuardSettings.("Configurations").keys|
		where {$_ -like "PSUnit?*"}).count
	#Act
    $Actual = ( $(Get-FuncGuardProject "PSUnit?*").count -eq $configCount )

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq $true}
}
################################################################################
##  - Set-FuncGuardProject
################################################################################
function Test.Set-FuncGuardProject_Invalid_NonExistingProject(
	[switch] $Category_Exported,
	[System.Management.Automation.ParameterBindingException] $ExpectedException `
	= "Actual: System.Management.Automation.ParameterBindingValidationException") {
   #Arrange

	#Act
    $Actual = Set-FuncGuardProject -Name NonExisting

	#Assert
}
function Test.Set-FuncGuardProject_Valid(
	[switch] $Category_Exported) {
    #Arrange
	$Configuration=@{"Name"="PSUnitSet";"Path"=$TMP_TESTFILES_PATH;
		"Prefix"="PSUNIT";"Type"="Custom"}
	$FuncGuardSettings.("Configurations").add("PSUnitSet",$Configuration)

	#Act
    Set-FuncGuardProject -Name PSUnitSet | Out-Null
	$Actual = ($FuncGuardSettings.("ActiveConfiguration") -eq "PSUnitSet")

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq $true}
}

################################################################################
##  - Remove-FuncGuardProject
################################################################################
function Test.Remove-FuncGuardProject_Invalid_NonExistingProject(
	[switch] $Category_Exported,
	[System.Management.Automation.ParameterBindingException] $ExpectedException `
	= "Actual: System.Management.Automation.ParameterBindingValidationException") {
   #Arrange

	#Act
    $Actual = Remove-FuncGuardProject -Name NonExisting

	#Assert
}
function Test.Remove-FuncGuardProject_Invalid_BuiltinProject(
	[switch] $Category_Exported,
	[System.ApplicationException] $ExpectedException = `
	"Actual: FuncGuardModuleException") {
   #Arrange

	#Act
    $Actual = Remove-FuncGuardProject -Name PSUnit

	#Assert
}
function Test.Remove-FuncGuardProject_Invalid_Active(
	[switch] $Category_Exported,
	[System.ApplicationException] $ExpectedException = `
	"Actual: FuncGuardModuleException") {
	#Arrange
	$FuncGuardSettings.("ActiveConfiguration") = "PSUnit"

	#Act
    $Actual = Remove-FuncGuardProject -Name PSUnit

	#Assert
}
function Test.Remove-FuncGuardProject_Valid(
	[switch] $Category_Exported) {
    #Arrange
	$Configuration=@{"Name"="PSUnitRemove";"Path"=$TMP_TESTFILES_PATH;
		"Prefix"="PSUNIT";"Type"="Custom"}
	$FuncGuardSettings.("Configurations").add("PSUnitRemove",$Configuration)

	#Act
	Remove-FuncGuardProject -Name "PSUnitRemove"
	$Actual = (! $FuncGuardSettings.("Configurations").containskey("PSUnitRemove"))

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq $true}
}

################################################################################
##  - Get-FuncGuardCoreFiles
################################################################################
function Test.Get-FuncGuardCoreFiles_ValidPathWithoutRequiredFiles(
	[switch] $Category_Internal) {
    #Arrange
	$Path = "C:\"

	#Act
	Get-FuncGuardCoreFiles -Path "$Path" -ea 0 -ev gfg_errors
	$Actual = $gfg_errors|sort -unique | measure-object | select -expand count

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq 3}
}
function Test.Get-FuncGuardCoreFiles_ValidPathWithoutSomeRequiredFiles(
	[switch] $Category_Internal) {
    #Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	Remove-Item -Force "$TMP_TESTFILES_PATH\FuncGuards_skel.h"

	#Act
    Get-FuncGuardCoreFiles -Path $TMP_TESTFILES_PATH -ea 0 -ev gfg_errors
	$Actual = $gfg_errors|sort -unique | measure-object | select -expand count

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq 1}
}
function Test.Get-FuncGuardCoreFiles_ValidPathWithRequiredFiles(
	[switch] $Category_Internal) {
    #Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable

	#Act
    $Actual = $(Get-FuncGuardCoreFiles -Path $TMP_TESTFILES_PATH).count

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq 3}
}

################################################################################
##  - Get-FuncGuardTemplateFiles
################################################################################
function Test.Get-FuncGuardTemplateFiles_ValidPathZeroFiles(
	[switch] $Category_Internal,
	[System.IO.FileNotFoundException] $ExpectedException = $Null) {
    #Arrange
	Clear-TmpFuncTestOptionalDirectory

	#Act
    $Actual = (Get-FuncGuardTemplateFiles -Path "$TMP_TESTFILES_PATH\templates")

	#Assert
}
function Test.Get-FuncGuardTemplateFiles_Valid(
	[switch] $Category_Internal) {
    #Arrange
	Test-OptionalFilesAvailable

	#Act
    $Actual = $(
		Get-FuncGuardTemplateFiles -Path "$TMP_TESTFILES_PATH\templates").count

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq 3}
}

################################################################################
##  - Test-Get-FuncGuardSourceFiles
################################################################################
function Test.Get-FuncGuardSourceFiles_Required(
	[switch]$Category_Internal) {

	#Arrange
	Test-RequiredFilesAvailable

	#Act
	$Actual = $(
		Get-FuncGuardSourceFiles -Source -SourcePath $TMP_TESTFILES_PATH).count

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq 3}
}
function Test.Get-FuncGuardSourceFiles_Optional(
	[switch]$Category_Internal) {

	#Arrange
	Test-RequiredFilesAvailable
	Test-OptionalFilesAvailable

	#Act
	$Actual = $(Get-FuncGuardSourceFiles -Template `
		-TemplatePath "$TMP_TESTFILES_PATH\templates").count

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq 3}
}
function Test.Get-FuncGuardSourceFiles_AllWithDefaultTemplatePath(
	[switch]$Category_Internal) {

	#Arrange
	Test-RequiredFilesAvailable
	Test-OptionalFilesAvailable

	#Act
	$Actual = $(Get-FuncGuardSourceFiles  -All `
		-SourcePath $TMP_TESTFILES_PATH).count

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq 6}
}
function Test.Get-FuncGuardSourceFiles_AllWithExplicitTemplatePath(
	[switch]$Category_Internal) {

	#Arrange
	Test-RequiredFilesAvailable
	Test-OptionalFilesAvailable

	#Act
	$Actual = $(Get-FuncGuardSourceFiles -All -SourcePath $TMP_TESTFILES_PATH `
		-TemplatePath "$TMP_TESTFILES_PATH\templates").count

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$ActualValue -eq 6}
}

################################################################################
##  - Add-FuncGuard
################################################################################
function Test.Add-FuncGuard_InvalidPath(
	[switch] $Category_Exported,
	[System.Management.Automation.ParameterBindingException] $ExpectedException `
	= "Actual: System.Management.Automation.ParameterBindingValidationException") {
	#Arrange
	$Prefix = "PSUnit"

	#Act
	$Actual = Add-FuncGuard -Prefix $Prefix -Path "C:\Invalid" `
		-Name Class3 -Description "For tracing Class3 functions."

	#Assert
}
function Test.Add-FuncGuard_FilePassedAsPath(
	[switch] $Category_Exported,
	[System.Management.Automation.ParameterBindingException] $ExpectedException `
	= "Actual: System.Management.Automation.ParameterBindingValidationException") {
	#Arrange
	$Prefix = "PSUnit"
	$Name = "Class3"
	$Description = "For tracing Class3 functions."

	#Act
	$Actual = Add-FuncGuard -Prefix $Prefix `
		-Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Name $Name -Description $Description

	#Assert
}
function Test.Add-FuncGuard_ActiveHeaderFileNotFound(
	[switch] $Category_Exported,
	[System.IO.FileNotFoundException] $ExpectedException = $null) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllEnabled
	$Prefix = "PSUnit"
	$Name = "Class3"
	$Description = "For tracing Class3 functions."

	#Act
	$Actual = Add-FuncGuard -Prefix $Prefix -Path $TMP_TESTFILES_PATH `
		-Name $Name -Description $Description

	#Assert
}
function Test.Add-FuncGuard_AddSimpleAtStart(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.h $HeaderContent_ThreeValidAllEnabled
	Set-Content $TMP_TESTFILES_PATH\FuncGuardsImport.h $ImporterContent
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.cpp $SourceContent

	$Prefix = "PSUnit"
	$Name = "Class0"
	$Description = "For tracing Class0 functions."

	#Act
	Add-FuncGuard -Prefix $Prefix -Path $TMP_TESTFILES_PATH `
		-Name $Name -Description $Description
	$AddedAtLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass0" -list | `
		select -ExpandProperty LineNumber)
	$NextGuardAt = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		 -Pattern "^\s*(//)*\s*func_guard\(guardClass1" -list | `
		 Select -ExpandProperty LineNumber)
	$Actual = @($AddedAtLine, $NextGuardAt)
	$Expected = @(19,23)

	#Assert
	Assert-That -ActualValue $Actual -Constraint {if (
		(Compare-Object $Actual $Expected) -eq $null) {$true} else {$false} }
}
function Test.Add-FuncGuard_AddSimpleInMiddle(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.h $HeaderContent_ThreeValidAllEnabled
	Set-Content $TMP_TESTFILES_PATH\FuncGuardsImport.h $ImporterContent
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.cpp $SourceContent
	$Prefix = "PSUnit"
	$Name = "Class1_Middle"
	$Description = "For tracing Class0 functions."

	#Act
	Add-FuncGuard -Prefix $Prefix -Path $TMP_TESTFILES_PATH `
		-Name $Name -Description $Description
	$PreviousGuardAt = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass1" -list | `
		select -ExpandProperty LineNumber)
	$AddedAtLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass1_Middle" -list | `
		select -ExpandProperty LineNumber)
	$NextGuardAt = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		 -Pattern "^\s*(//)*\s*func_guard\(guardClass1Class2_Communications" `
		 -list | Select -ExpandProperty LineNumber)
	$Actual = @($PreviousGuardAt, $AddedAtLine, $NextGuardAt)
	$Expected = @(19,23,27)

	#Assert
	Assert-That -ActualValue $Actual -Constraint {if (
		(Compare-Object $Actual $Expected) -eq $null) {$true} else {$false} }
}
function Test.Add-FuncGuard_AddSimpleAtEnd(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.h $HeaderContent_ThreeValidAllEnabled
	Set-Content $TMP_TESTFILES_PATH\FuncGuardsImport.h $ImporterContent
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.cpp $SourceContent
	$Prefix = "PSUnit"
	$Name = "Class3"
	$Description = "For tracing Class3 functions."

	#Act
	Add-FuncGuard -Prefix $Prefix -Path $TMP_TESTFILES_PATH `
		-Name $Name -Description $Description
	$PreviousGuardAt = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass2" -list | `
		select -ExpandProperty LineNumber)
	$AddedAtLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass3" -list | `
		select -ExpandProperty LineNumber)
	$Actual = @($PreviousGuardAt, $AddedAtLine)
	$Expected = @(40,44)

	#Assert
	Assert-That -ActualValue $Actual -Constraint {if (
		(Compare-Object $Actual $Expected) -eq $null) {$true} else {$false} }
}
function Test.Add-FuncGuard_AddSimpleToEmpty(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.h $HeaderContent_Empty
	Set-Content $TMP_TESTFILES_PATH\FuncGuardsImport.h $ImporterContent_Empty
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.cpp $SourceContent_Empty
	$Prefix = "PSUnit"
	$Name = "Class3"
	$Description = "For tracing Class3 functions."

	#Act
	$PreviousEndAt = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "#endif // SHIPPING" -list | `
		select -ExpandProperty LineNumber)
	Add-FuncGuard -Prefix $Prefix -Path $TMP_TESTFILES_PATH `
		-Name $Name -Description $Description
	$AddedAtLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass3" -list | `
		select -ExpandProperty LineNumber)
	$Actual = @($PreviousEndAt, $AddedAtLine)
	$Expected = @(19,19)

	#Assert
	Assert-That -ActualValue $Actual -Constraint {if (
		(Compare-Object $Actual $Expected) -eq $null) {$true} else {$false} }
}
function Test.Add-FuncGuard_AddWithInstreamDescriptionAtStart(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.h $HeaderContent_ThreeValidAllEnabled
	Set-Content $TMP_TESTFILES_PATH\FuncGuardsImport.h $ImporterContent
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.cpp $SourceContent
	$Prefix = "PSUnit"
	$Name = "Class0"
	$InstreamPart = @"
/*
	An instream description
*/
"@
	$Description = "For tracing Class0 functions."
	$Description += "`r`n" + $InstreamPart

	#Act
	Add-FuncGuard -Prefix $Prefix -Path $TMP_TESTFILES_PATH `
		-Name $Name -Description $Description
	$AddedAtLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass0" -list | `
		select -ExpandProperty LineNumber)
	$NextGuardAt = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		 -Pattern "^\s*(//)*\s*func_guard\(guardClass1" -list | `
		 Select -ExpandProperty LineNumber)
	$Actual = @($AddedAtLine, $NextGuardAt)
	$Expected = @(19,26)

	#Assert
	Assert-That -ActualValue $Actual -Constraint {if (
		(Compare-Object $Actual $Expected) -eq $null) {$true} else {$false} }
}
function Test.Add-FuncGuard_AddWithInstreamDescriptionInMiddle(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.h $HeaderContent_ThreeValidAllEnabled
	Set-Content $TMP_TESTFILES_PATH\FuncGuardsImport.h $ImporterContent
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.cpp $SourceContent
	$Prefix = "PSUnit"
	$Name = "Class1_Middle"
	$InstreamPart = @"
/*
	An instream description
*/
"@
	$Description = "For tracing Class1 functions."
	$Description += "`r`n" + $InstreamPart

	#Act
	Add-FuncGuard -Prefix $Prefix -Path $TMP_TESTFILES_PATH `
		-Name $Name -Description $Description
	$PreviousGuardAt = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass1" -list | `
		select -ExpandProperty LineNumber)
	$AddedAtLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass1_Middle" -list | `
		select -ExpandProperty LineNumber)
	$NextGuardAt = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		 -Pattern "^\s*(//)*\s*func_guard\(guardClass1Class2_Communications" `
		 -list | Select -ExpandProperty LineNumber)
	$Actual = @($PreviousGuardAt, $AddedAtLine, $NextGuardAt)
	$Expected = @(19,23,30)

	#Assert
	Assert-That -ActualValue $Actual -Constraint {if (
		(Compare-Object $Actual $Expected) -eq $null) {$true} else {$false} }
}
function Test.Add-FuncGuard_AddWithInstreamDescriptionEnd(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.h $HeaderContent_ThreeValidAllEnabled
	Set-Content $TMP_TESTFILES_PATH\FuncGuardsImport.h $ImporterContent
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.cpp $SourceContent
	$Prefix = "PSUnit"
	$Name = "Class3"
	$Description = "For tracing Class3 functions."

	#Act
	Add-FuncGuard -Prefix $Prefix -Path $TMP_TESTFILES_PATH `
		-Name $Name -Description $Description
	$PreviousGuardAt = (Get-Content $TMP_TESTFILES_PATH\FuncGuards.h | `
		Select-String -Pattern "^\s*(//)*\s*func_guard\(guardClass2").LineNumber
	$AddedAtLine = (Get-Content $TMP_TESTFILES_PATH\FuncGuards.h | `
		Select-String -Pattern "^\s*(//)*\s*func_guard\(guardClass3").LineNumber
	$PreviousGuardAt = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass2" -list | `
		select -ExpandProperty LineNumber)
	$AddedAtLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass3" -list | `
		select -ExpandProperty LineNumber)
	$Actual = @($PreviousGuardAt, $AddedAtLine)
	$Expected = @(40,44)

	#Assert
	Assert-That -ActualValue $Actual -Constraint {if (
		(Compare-Object $Actual $Expected) -eq $null) {$true} else {$false} }
}
function Test.Add-FuncGuard_UseProjectSettings(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.h $HeaderContent_ThreeValidAllEnabled
	Set-Content $TMP_TESTFILES_PATH\FuncGuardsImport.h $ImporterContent
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.cpp $SourceContent
	$Name = "Class3"
	$Description = "For tracing Class3 functions."

	#Act
	Add-FuncGuard -Name $Name -Description $Description
	$PreviousGuardAt = (Get-Content $TMP_TESTFILES_PATH\FuncGuards.h | `
		Select-String -Pattern "^\s*(//)*\s*func_guard\(guardClass2").LineNumber
	$AddedAtLine = (Get-Content $TMP_TESTFILES_PATH\FuncGuards.h | `
		Select-String -Pattern "^\s*(//)*\s*func_guard\(guardClass3").LineNumber
	$PreviousGuardAt = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass2" -list | `
		select -ExpandProperty LineNumber)
	$AddedAtLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass3" -list | `
		select -ExpandProperty LineNumber)
	$Actual = @($PreviousGuardAt, $AddedAtLine)
	$Expected = @(40,44)

	#Assert
	Assert-That -ActualValue $Actual -Constraint {if (
		(Compare-Object $Actual $Expected) -eq $null) {$true} else {$false} }
}

################################################################################
##  - Get-FuncGuard
################################################################################
function Test.Get-FuncGuard_InvalidPath(
	[switch] $Category_Exported,
	[System.Management.Automation.ParameterBindingException] $ExpectedException `
	= "Actual: System.Management.Automation.ParameterBindingValidationException") {
	#Arrange

	#Act
	$Actual = Get-FuncGuard -Path c:\Invalid

	#Assert
}
function Test.Get-FuncGuard_FilePassedAsPath(
	[switch] $Category_Exported,
	[System.Management.Automation.ParameterBindingException] $ExpectedException `
	= "Actual: System.Management.Automation.ParameterBindingValidationException") {
	#Arrange

	#Act
	$Actual = Get-FuncGuard -Path $TMP_TESTFILES_PATH\Invalid.h

	#Assert
}
function Test.Get-FuncGuard_ActiveFileNotFound(
	[switch] $Category_Exported,
	[System.IO.FileNotFoundException] $ExpectedException = $null) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled

	#Act
	$Actual = Get-FuncGuard -Path $TMP_TESTFILES_PATH

	#Assert
}
function Test.Get-FuncGuard_UseProjectPath(
	[switch] $Category_Exported) {
	#Arrange
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	$Actual = (Get-FuncGuard -Name *).Count

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq 3}
}
function Test.Get-FuncGuard_ActiveFileAllEntries(
	[switch] $Category_Exported) {
	#Arrange
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	$Actual = (Get-FuncGuard -Name * -Path $TMP_TESTFILES_PATH).Count

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq 3}
}
function Test.Get-FuncGuard_NamedFuncGuardNotFound_Stop(
	[switch] $Category_Exported,
	[System.ApplicationException] $ExpectedException = `
	"Actual: FuncGuardModuleException") {
	#Arrange
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled

	#Act
	$Actual = Get-FuncGuard -Name Class3 -Path $TMP_TESTFILES_PATH `
		-Skeleton -ErrorAction "Stop"

	#Assert
}
function Test.Get-FuncGuard_ExactMatchFound(
	[switch] $Category_Exported) {
	#Arrange
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled

	#Act
	$Actual = (Get-FuncGuard -Name Class1 -Path $TMP_TESTFILES_PATH `
		-Skeleton).Name

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq "Class1"}
}
function Test.Get-FuncGuard_SingleWildCardMatchFound(
	[switch] $Category_Exported) {
	#Arrange
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled

	#Act
	$Actual = (Get-FuncGuard -Name *1 -Path $TMP_TESTFILES_PATH `
		-Skeleton).Name

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq "Class1"}
}
function Test.Get-FuncGuard_MultiWildCardMatchFound(
	[switch] $Category_Exported) {
	#Arrange
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled

	#Act
	$Actual = (Get-FuncGuard -Name Class1* -Path $TMP_TESTFILES_PATH `
		-Skeleton).Count

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq 2}
}
function Test.Get-FuncGuard_NoNameSpecified(
	[switch] $Category_Exported) {
	#Arrange
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled

	#Act
	$Actual = (Get-FuncGuard -Path $TMP_TESTFILES_PATH -Skeleton).Count

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq 3}
}
function Test.Get-FuncGuard_FormatListFullDescription(
	[switch] $Category_Exported) {
	#Arrange
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled

	#Act
	$Actual = Get-FuncGuard Class1?* -Path $TMP_TESTFILES_PATH -Skeleton | `
		select -ExpandProperty FullDescription | measure -Line | `
		select -ExpandProperty Lines

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq 14}
}

################################################################################
##  - Enable-FuncGuard
################################################################################
function Test.Enable-FuncGuard_InvalidPath(
	[switch] $Category_Exported,
	[System.Management.Automation.ParameterBindingException] $ExpectedException `
	= "Actual: System.Management.Automation.ParameterBindingValidationException") {
	#Arrange

	#Act
	$Actual = Enable-FuncGuard -Path C:\Invalid

	#Assert
}
function Test.Enable-FuncGuard_FilePassedAsPath(
	[switch] $Category_Exported,
	[System.Management.Automation.ParameterBindingException] $ExpectedException `
	= "Actual: System.Management.Automation.ParameterBindingValidationException") {
	#Arrange

	#Act
	$Actual = Disable-FuncGuard -Path $TMP_TESTFILES_PATH\Invalid.h

	#Assert
}
function Test.Enable-FuncGuard_ActiveHeaderFileNotFound(
	[switch] $Category_Exported,
	[System.IO.FileNotFoundException] $ExpectedException = $null) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled

	#Act
	$Actual = Enable-FuncGuard -Path $TMP_TESTFILES_PATH

	#Assert
}
function Test.Enable-FuncGuard_NoNameSpecified(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	Enable-FuncGuard -Path $TMP_TESTFILES_PATH
	$Actual = (Get-FuncGuard -Path $TMP_TESTFILES_PATH | `
		Where-Object {$_.Status -eq "Enabled"} | Measure-Object).Count

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq 3}
}
function Test.Enable-FuncGuard_NamedFuncGuardNotFound(
	[switch] $Category_Exported,
	[System.ApplicationException] $ExpectedException = `
	"Actual: FuncGuardModuleException") {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	$Actual = Enable-FuncGuard -Name Class3 -Path $TMP_TESTFILES_PATH `
		-ErrorAction "Stop"

	#Assert
}
function Test.Enable-FuncGuard_ExactMatchFoundUsingProjectPath(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	Enable-FuncGuard -Name Class1
	$Actual = (Get-FuncGuard -Path $TMP_TESTFILES_PATH | `
		Where-Object {$_.Status -eq "Enabled"} | Measure-Object).Count

	#Assert
	Assert-That -ActualValue $Actual -Constraint { $Actual -eq 1 }
}
function Test.Enable-FuncGuard_ExactMatchFound(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	Enable-FuncGuard -Name Class1 -Path $TMP_TESTFILES_PATH
	$Actual = (Get-FuncGuard -Path $TMP_TESTFILES_PATH | `
		Where-Object {$_.Status -eq "Enabled"} | Measure-Object).Count

	#Assert
	Assert-That -ActualValue $Actual -Constraint { $Actual -eq 1 }
}
function Test.Enable-FuncGuard_SingleWildCardMatchFound(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	Enable-FuncGuard -Name *1 -Path $TMP_TESTFILES_PATH
	$Actual = (Get-FuncGuard -Path $TMP_TESTFILES_PATH | `
		Where-Object {$_.Status -eq "Enabled"})

	#Assert
	Assert-That -ActualValue $Actual -Constraint { (
		($Actual.Name -eq "Class1") -and
		(($Actual|Measure-Object).Count -eq 1)) }
}
function Test.Enable-FuncGuard_MultiWildCardMatchFound(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	Enable-FuncGuard -Name Class1* -Path $TMP_TESTFILES_PATH
	$Actual = @(Get-FuncGuard -Path $TMP_TESTFILES_PATH | `
		Where-Object {$_.Status -eq "Enabled"})

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual.Count -eq 2}
}
function Test.Enable-FuncGuard_SingleMatchFoundAlreadyEnabled(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	Enable-FuncGuard -Name Class1 -Path $TMP_TESTFILES_PATH
	$Actual = @(Enable-FuncGuard -Name Class1 -Path $TMP_TESTFILES_PATH)

	#Assert
	Assert-That -ActualValue $Actual -Constraint { (($Actual.Count -eq 1 ) -and
		($Actual[0].Name -eq "Class1")) }
}
function Test.Enable-FuncGuard_MultiWildCardMatchFound_SingleAlreadyEnabled(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	Enable-FuncGuard -Name Class1 -Path $TMP_TESTFILES_PATH
	$Actual = @(Enable-FuncGuard -Name Class1* -Path $TMP_TESTFILES_PATH)

	#Assert
	Assert-That -ActualValue $Actual -Constraint { (($Actual.Count -eq 2 ) -and
		($Actual[0].Name -eq "Class1") -and
		($Actual[1].Name -eq "Class1Class2_Communications"))}
}
function Test.Enable-FuncGuard_MultiWildCardMatchFoundAlreadyEnabled(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	Enable-FuncGuard -Name Class1* -Path $TMP_TESTFILES_PATH
	$Actual = @(Enable-FuncGuard -Name Class1* -Path $TMP_TESTFILES_PATH)

	#Assert
	Assert-That -ActualValue $Actual -Constraint { (($Actual.Count -eq 2 ) -and
		($Actual[0].Name -eq "Class1") -and
		($Actual[1].Name -eq "Class1Class2_Communications"))}
}

################################################################################
##  - Disable-FuncGuard
################################################################################
function Test.Disable-FuncGuard_InvalidPath(
	[switch] $Category_Exported,
	[System.Management.Automation.ParameterBindingException] $ExpectedException `
	= "Actual: System.Management.Automation.ParameterBindingValidationException") {
	#Arrange

	#Act
	$Actual = Disable-FuncGuard -Path C:\Invalid

	#Assert
}
function Test.Disable-FuncGuard_InvalidFilePassedAsPath(
	[switch] $Category_Exported,
	[System.Management.Automation.ParameterBindingException] $ExpectedException `
	= "Actual: System.Management.Automation.ParameterBindingValidationException") {
	#Arrange

	#Act
	$Actual = Disable-FuncGuard -Path $TMP_TESTFILES_PATH\Invalid.h

	#Assert
}
function Test.Disable-FuncGuard_ActiveHeaderFileNotFound(
	[switch] $Category_Exported,
	[System.IO.FileNotFoundException] $ExpectedException = $null) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllEnabled

	#Act
	$Actual = Disable-FuncGuard -Path $TMP_TESTFILES_PATH

	#Assert
}
function Test.Disable-FuncGuard_NoNameSpecified(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllEnabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	Disable-FuncGuard -Path $TMP_TESTFILES_PATH
	$Actual = (Get-FuncGuard -Path $TMP_TESTFILES_PATH | `
		Where-Object {$_.Status -eq "Disabled"} | Measure-Object).Count

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq 3}
}
function Test.Disable-FuncGuard_NamedFuncGuardNotFound(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllEnabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	Disable-FuncGuard -Name Class3 -Path $TMP_TESTFILES_PATH `
		-ErrorVariable err -ErrorAction "SilentlyContinue"
	$Actual = ( ( $err[0].CategoryInfo.Category -eq "ObjectNotFound" ) -and
		( $err[0].Exception.Message -eq
			"Cannot disable 'Class3' because it does not exist.`n" ) )

	#Assert
	Assert-That -ActualValue $Actual -Constraint { $Actual -eq $true }

}
function Test.Disable-FuncGuard_ExactMatchFoundUsingProjectPath(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllEnabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	Disable-FuncGuard -Name Class1
	$Actual = (Get-FuncGuard -Path $TMP_TESTFILES_PATH | `
		Where-Object {$_.Status -eq "Disabled"} | Measure-Object).Count

	#Assert
	Assert-That -ActualValue $Actual -Constraint { $Actual -eq 1 }
}
function Test.Disable-FuncGuard_ExactMatchFound(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllEnabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	Disable-FuncGuard -Name Class1 -Path $TMP_TESTFILES_PATH
	$Actual = (Get-FuncGuard -Path $TMP_TESTFILES_PATH | `
		Where-Object {$_.Status -eq "Disabled"} | Measure-Object).Count

	#Assert
	Assert-That -ActualValue $Actual -Constraint { $Actual -eq 1 }
}
function Test.Disable-FuncGuard_SingleWildCardMatchFound(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllEnabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	Disable-FuncGuard -Name *1 -Path $TMP_TESTFILES_PATH
	$Actual = (Get-FuncGuard -Path $TMP_TESTFILES_PATH | `
		Where-Object {$_.Status -eq "Disabled"})

	#Assert
	Assert-That -ActualValue $Actual -Constraint { (
		($Actual.Name -eq "Class1") -and
		(($Actual|Measure-Object).Count -eq 1)) }
}
function Test.Disable-FuncGuard_MultiWildCardMatchFound(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllEnabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	Disable-FuncGuard -Name Class1* -Path $TMP_TESTFILES_PATH
	$Actual = (Get-FuncGuard -Path $TMP_TESTFILES_PATH | `
		Where-Object {$_.Status -eq "Disabled"}|measure-object).Count

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq 2}
}
function Test.Disable-FuncGuard_SingleMatchFoundAlreadyDisabled(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllEnabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	Disable-FuncGuard -Name Class1 -Path $TMP_TESTFILES_PATH
	$Actual = @(Disable-FuncGuard -Name Class1 -Path $TMP_TESTFILES_PATH)

	#Assert
	Assert-That -ActualValue $Actual -Constraint { (($Actual.Count -eq 1 ) -and
		($Actual[0].Name -eq "Class1"))}
}
function Test.Disable-FuncGuard_MultiWildCardMatchFound_SingleAlreadyDisabled(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllEnabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	Disable-FuncGuard -Name Class1 -Path $TMP_TESTFILES_PATH
	$Actual = @(Disable-FuncGuard -Name Class1* -Path $TMP_TESTFILES_PATH)

	#Assert
	Assert-That -ActualValue $Actual -Constraint { (($Actual.Count -eq 2 ) -and
		($Actual[0].Name -eq "Class1") -and
		($Actual[1].Name -eq "Class1Class2_Communications"))}
}
function Test.Disable-FuncGuard_MultiWildCardMatchFoundAlreadyDisabled(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllEnabled
	Copy-Item $TargetPath "$TMP_TESTFILES_PATH\FuncGuards.h"

	#Act
	Disable-FuncGuard -Name Class1* -Path $TMP_TESTFILES_PATH
	$Actual = @(Disable-FuncGuard -Name Class1* -Path $TMP_TESTFILES_PATH)

	#Assert
	Assert-That -ActualValue $Actual -Constraint { (($Actual.Count -eq 2 ) -and
		($Actual[0].Name -eq "Class1") -and
		($Actual[1].Name -eq "Class1Class2_Communications"))}
}

################################################################################
##  - Remove-FuncGuard
################################################################################
function Test.Remove-FuncGuard_InvalidPath(
	[switch] $Category_Exported,
	[System.Management.Automation.ParameterBindingException] $ExpectedException `
	= "Actual: System.Management.Automation.ParameterBindingValidationException") {
	#Arrange

	#Act
	$Actual = Remove-FuncGuard -Path "C:\Invalid" -Name Class3

	#Assert
}
function Test.Remove-FuncGuard_FilePassedAsPath(
	[switch] $Category_Exported,
	[System.IO.FileNotFoundException] $ExpectedException = $null) {
	#Arrange
	$Name = "Class3"

	#Act
	$Actual = Remove-FuncGuard -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Name $Name -ea 0

	#Assert
}
function Test.Remove-FuncGuard_ActiveHeaderFileNotFound(
	[switch] $Category_Exported,
	[System.IO.FileNotFoundException] $ExpectedException = $null) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllEnabled
	$Name = "Class3"

	#Act
	$Actual = Remove-FuncGuard -Path $TMP_TESTFILES_PATH -Name $Name

	#Assert
}
function Test.Remove-FuncGuard_RemoveAtStartUsingProjectPath(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.h $HeaderContent_ThreeValidAllEnabled
	Set-Content $TMP_TESTFILES_PATH\FuncGuardsImport.h $ImporterContent
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.cpp $SourceContent

	#Act
	$RemoveFromHeaderLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass1" -list | `
		select -ExpandProperty LineNumber)
	$RemoveFromSourceLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.cpp `
		-Pattern "^#ifdef FUNCGUARD_PSUNIT_CLASS1" -list | `
		select -ExpandProperty LineNumber)
	$RemoveFromImporterLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuardsImport.h `
		-Pattern "^func_guard_import_cat\(guardClass1\)" -list | `
		select -ExpandProperty LineNumber)

	Remove-FuncGuard -Name Class1

	$NextGuardNowAtHeaderLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass1Class2_Communications" -list | `
		select -ExpandProperty LineNumber)
	$NextGuardNowAtSourceLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.cpp `
		-Pattern "^#ifdef FUNCGUARD_PSUNIT_CLASS1CLASS2_COMMUNICATIONS" -list | `
		select -ExpandProperty LineNumber)
	$NextGuardNowAtImporterLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuardsImport.h `
		-Pattern "^func_guard_import_cat\(guardClass1Class2_Communications\)" -list | `
		select -ExpandProperty LineNumber)
	$Actual = ( ($RemoveFromHeaderLine -eq $NextGuardNowAtHeaderLine) -and
		( $RemoveFromSourceLine -eq $NextGuardNowAtSourceLine ) -and
		( $RemoveFromImporterLine -eq $NextGuardNowAtImporterLine ) )

	#Assert
	Assert-That -ActualValue $Actual -Constraint { $Actual -eq $true }
}
function Test.Remove-FuncGuard_RemoveAtStart(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.h $HeaderContent_ThreeValidAllEnabled
	Set-Content $TMP_TESTFILES_PATH\FuncGuardsImport.h $ImporterContent
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.cpp $SourceContent

	#Act
	$RemoveFromHeaderLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass1" -list | `
		select -ExpandProperty LineNumber)
	$RemoveFromSourceLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.cpp `
		-Pattern "^#ifdef FUNCGUARD_PSUNIT_CLASS1" -list | `
		select -ExpandProperty LineNumber)
	$RemoveFromImporterLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuardsImport.h `
		-Pattern "^func_guard_import_cat\(guardClass1\)" -list | `
		select -ExpandProperty LineNumber)

	Remove-FuncGuard -Path $TMP_TESTFILES_PATH -Name Class1

	$NextGuardNowAtHeaderLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass1Class2_Communications" -list | `
		select -ExpandProperty LineNumber)
	$NextGuardNowAtSourceLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.cpp `
		-Pattern "^#ifdef FUNCGUARD_PSUNIT_CLASS1CLASS2_COMMUNICATIONS" -list | `
		select -ExpandProperty LineNumber)
	$NextGuardNowAtImporterLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuardsImport.h `
		-Pattern "^func_guard_import_cat\(guardClass1Class2_Communications\)" -list | `
		select -ExpandProperty LineNumber)
	$Actual = ( ($RemoveFromHeaderLine -eq $NextGuardNowAtHeaderLine) -and
		( $RemoveFromSourceLine -eq $NextGuardNowAtSourceLine ) -and
		( $RemoveFromImporterLine -eq $NextGuardNowAtImporterLine ) )

	#Assert
	Assert-That -ActualValue $Actual -Constraint { $Actual -eq $true }
}
function Test.Remove-FuncGuard_RemoveInMiddle(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.h $HeaderContent_ThreeValidAllEnabled
	Set-Content $TMP_TESTFILES_PATH\FuncGuardsImport.h $ImporterContent
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.cpp $SourceContent

	#Act
	$OrigClass1Line = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass1" -list | `
		select -ExpandProperty LineNumber)
	$RemoveFromHeaderLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass1Class2_Communications" -list | `
		select -ExpandProperty LineNumber)
	$RemoveFromSourceLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.cpp `
		-Pattern "^#ifdef FUNCGUARD_PSUNIT_CLASS1CLASS2_COMMUNICATIONS" -list | `
		select -ExpandProperty LineNumber)
	$RemoveFromImporterLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuardsImport.h `
		-Pattern "^func_guard_import_cat\(guardClass1Class2_Communications\)" -list | `
		select -ExpandProperty LineNumber)

	Remove-FuncGuard -Path $TMP_TESTFILES_PATH -Name Class1Class2_Communications

	$FinalClass1Line = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass1" -list | `
		select -ExpandProperty LineNumber)
	$NextGuardNowAtHeaderLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass2" -list | `
		select -ExpandProperty LineNumber)
	$NextGuardNowAtSourceLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.cpp `
		-Pattern "^#ifdef FUNCGUARD_PSUNIT_CLASS2" -list | `
		select -ExpandProperty LineNumber)
	$NextGuardNowAtImporterLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuardsImport.h `
		-Pattern "^func_guard_import_cat\(guardClass2\)" -list | `
		select -ExpandProperty LineNumber)
	$Actual = ( ($RemoveFromHeaderLine -eq $NextGuardNowAtHeaderLine) -and
		( $RemoveFromSourceLine -eq $NextGuardNowAtSourceLine ) -and
		( $RemoveFromImporterLine -eq $NextGuardNowAtImporterLine ) -and
		( $OrigClass1Line -eq $FinalClass1Line ) )

	#Assert
	Assert-That -ActualValue $Actual -Constraint { $Actual -eq $true }
}
function Test.Remove-FuncGuard_RemoveAtEnd(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.h $HeaderContent_ThreeValidAllEnabled
	Set-Content $TMP_TESTFILES_PATH\FuncGuardsImport.h $ImporterContent
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.cpp $SourceContent

	#Act
	$OrigEndMarkerLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuardsImport.h `
		-SimpleMatch "#endif // MISC_DEBUG_FUNCGUARDS_H" -list | `
		select -ExpandProperty LineNumber)
	$PrevGuardHeaderLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass1Class2_Communications" -list | `
		select -ExpandProperty LineNumber)
	$PrevGuardSourceLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.cpp `
		-Pattern "^#ifdef FUNCGUARD_PSUNIT_CLASS1CLASS2_COMMUNICATIONS" -list | `
		select -ExpandProperty LineNumber)
	$PrevGuardImporterLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuardsImport.h `
		-Pattern "^func_guard_import_cat\(guardClass1Class2_Communications\)" -list | `
		select -ExpandProperty LineNumber)

	Remove-FuncGuard -Path $TMP_TESTFILES_PATH -Name Class2

	$FinalEndMarkerLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuardsImport.h `
		-SimpleMatch "#endif // MISC_DEBUG_FUNCGUARDS_H" -list | `
		select -ExpandProperty LineNumber)
	$PrevGuardNowAtHeaderLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass1Class2_Communications" -list | `
		select -ExpandProperty LineNumber)
	$PrevGuardNowAtSourceLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.cpp `
		-Pattern "^#ifdef FUNCGUARD_PSUNIT_CLASS1CLASS2_COMMUNICATIONS" -list | `
		select -ExpandProperty LineNumber)
	$PrevGuardNowAtImporterLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuardsImport.h `
		-Pattern "^func_guard_import_cat\(guardClass1Class2_Communications\)" -list | `
		select -ExpandProperty LineNumber)
	$Actual = ( ($PrevGuardHeaderLine -eq $PrevGuardNowAtHeaderLine) -and
		( $PrevGuardSourceLine -eq $PrevGuardNowAtSourceLine ) -and
		( $PrevGuardImporterLine -eq $PrevGuardNowAtImporterLine ) -and
		( $OrigEndMarkerLine -eq ( $FinalEndMarkerLine + 1 ) ) )

	#Assert
	Assert-That -ActualValue $Actual -Constraint { $Actual -eq $true }
}
function Test.Remove-FuncGuard_RemoveFromSingle(
	[switch] $Category_Exported) {
	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.h $HeaderContent_Single
	Set-Content $TMP_TESTFILES_PATH\FuncGuardsImport.h $ImporterContent_Single
	Set-Content $TMP_TESTFILES_PATH\FuncGuards.cpp $SourceContent_Single

	#Act
	$PrevGuardHeaderLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "^\s*(//)*\s*func_guard\(guardClass1" -list | `
		select -ExpandProperty LineNumber)
	$PrevGuardSourceLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.cpp `
		-Pattern "^#ifdef FUNCGUARD_PSUNIT_CLASS1" -list | `
		select -ExpandProperty LineNumber)
	$PrevGuardImporterLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuardsImport.h `
		-Pattern "^func_guard_import_cat\(guardClass1\)" -list | `
		select -ExpandProperty LineNumber)

	Remove-FuncGuard -Path $TMP_TESTFILES_PATH -Name Class1

	$FinalEndHeaderLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.h `
		-Pattern "#endif // SHIPPING" -list | `
		select -ExpandProperty LineNumber)
	$FinalEndSourceLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuards.cpp `
		-Pattern "// FUNCGUARD_BLOCK_END" -list | `
		select -ExpandProperty LineNumber)
	$FinalEndImporterLine = $(Select-String -Path $TMP_TESTFILES_PATH\FuncGuardsImport.h `
		-Pattern "#endif // MISC_DEBUG_FUNCGUARDS_H" -list | `
		select -ExpandProperty LineNumber)
	$Actual = ( ($PrevGuardHeaderLine -eq ($FinalEndHeaderLine)) -and
		( $PrevGuardSourceLine -eq ($FinalEndSourceLine) ) -and
		( $PrevGuardImporterLine -eq ($FinalEndImporterLine) ) )

	#Assert
	Assert-That -ActualValue $Actual -Constraint { $Actual -eq $true }
}

################################################################################
##  - Update-FuncGuardFiles
################################################################################

function prepupdateHeaderContent {
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$TargetPath="$TMP_TESTFILES_PATH\FuncGuards_skel.h"
	Set-Content $TargetPath $HeaderContent_ThreeValidAllDisabled
	Copy-Item $TargetPath $TMP_TESTFILES_PATH\FuncGuards.h
}
function getTestRawCodeBlock {
	param(
		[Parameter(Position=0, Mandatory=$true)]
		[string] $Name
	,
		[Parameter(Position=1, Mandatory=$false)]
		[switch] $Instream = $false
	)
	$RawCodeBlock = @"
// func_guard(guard$Name);
// Description: Tracing for $Name functions.$(if($Instream){"`r`n/*`r`n`tAn instream description`r`n*/"})
//#define FUNCGUARD_PSUNIT_$($Name.ToUpper())
"@
	return $RawCodeBlock
}
function Test.Update-FuncGuardFiles_InvalidSourcePath(
	[switch] $Category_Exported,
	[System.Management.Automation.ParameterBindingException] $ExpectedException `
	= "Actual: System.Management.Automation.ParameterBindingValidationException") {
	#Arrange

	#Act
	$Actual = Update-FuncGuardFiles -Path "C:\Invalid" -Skeleton

	#Assert
}
function Test.Update-FuncGuardFiles_InvalidTemplatePath(
	[switch] $Category_Exported,
	[System.Management.Automation.ParameterBindingException] $ExpectedException `
	= "Actual: System.Management.Automation.ParameterBindingValidationException") {
	#Arrange

	#Act
	$Actual = Update-FuncGuardFiles -Path $TMP_TESTFILES_PATH `
		-TemplatePath c:\Invalid -Template

	#Assert
}
function Test.Update-FuncGuardFiles_DefaultSkeleton_UsingProjectPath(
	[switch] $Category_Exported) {

	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$OFS = [Environment]::NewLine
	Set-Content "$TMP_TESTFILES_PATH\FuncGuards.h" $HeaderContent_ThreeValidAllEnabled
	Set-Content "$TMP_TESTFILES_PATH\FuncGuards_skel.h" $HeaderContent_Empty

	#Act
	Update-FuncGuardFiles
	[string]$Actual = Get-Content $TMP_TESTFILES_PATH\FuncGuards_skel.h

	#Assert
	Assert-That -ActualValue $Actual -Constraint {if (
		(Compare-Object $Actual $HeaderContent_ThreeValidAllDisabled) -eq $null) `
		{$true} else {$false} }
}
function Test.Update-FuncGuardFiles_Skeleton_PathSpecified(
	[switch] $Category_Exported) {

	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	$OFS = [Environment]::NewLine
	Set-Content "$TMP_TESTFILES_PATH\FuncGuards.h" $HeaderContent_ThreeValidAllEnabled
	Set-Content "$TMP_TESTFILES_PATH\FuncGuards_skel.h" $HeaderContent_Empty

	#Act
	Update-FuncGuardFiles -Skeleton -Path $TMP_TESTFILES_PATH
	[string]$Actual = Get-Content $TMP_TESTFILES_PATH\FuncGuards_skel.h

	#Assert
	Assert-That -ActualValue $Actual -Constraint {if (
		(Compare-Object $Actual $HeaderContent_ThreeValidAllDisabled) -eq $null) `
		{$true} else {$false} }
}
function Test.Update-FuncGuardFiles_Templates_TemplatePathSpecified(
	[switch] $Category_Exported) {

	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	Clear-TmpFuncTestOptionalDirectory
	Test-OptionalFilesAvailable
	$OFS = [Environment]::NewLine
	Set-Content "$TMP_TESTFILES_PATH\FuncGuards.h" $HeaderContent_ThreeValidAllEnabled
	Set-Content "$TMP_TESTFILES_PATH\FuncGuards_skel.h" $HeaderContent_Empty
	Set-Content "$TMP_TESTFILES_PATH\templates\FuncGuards1_template.h" $HeaderContent_SingleEnabled
	Set-Content "$TMP_TESTFILES_PATH\\templates\FuncGuards2_template.h" $HeaderContent_FourValidAllEnabled
	Set-Content "$TMP_TESTFILES_PATH\\templates\FuncGuards3_template.h" $HeaderContent_Empty

	#Act
	Update-FuncGuardFiles -Template -Path $TMP_TESTFILES_PATH `
		-TemplatePath $TMP_TESTFILES_PATH\templates -wa 0
	[string]$skelContent = Get-Content $TMP_TESTFILES_PATH\FuncGuards_skel.h
	[string]$T1Content = Get-Content $TMP_TESTFILES_PATH\templates\FuncGuards1_template.h
	[string]$T2Content = Get-Content $TMP_TESTFILES_PATH\\templates\FuncGuards2_template.h
	[string]$T3Content = Get-Content $TMP_TESTFILES_PATH\templates\FuncGuards3_template.h

	$Actual = (
		((Compare-Object $skelContent $HeaderContent_Empty) -eq $null) -and
		((Compare-Object $T1Content $HeaderContent_ThreeValidSingleEnabled) -eq $null) -and
		((Compare-Object $T2Content $HeaderContent_ThreeValidAllEnabled) -eq $null) -and
		((Compare-Object $T3Content $HeaderContent_ThreeValidAllDisabled) -eq $null) )

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq $true}
}
function Test.Update-FuncGuardFiles_All_UsingProjectPath(
	[switch] $Category_Exported) {

	#Arrange
	Clear-TmpFuncTestDirectory
	Test-RequiredFilesAvailable
	Clear-TmpFuncTestOptionalDirectory
	Test-OptionalFilesAvailable
	$OFS = [Environment]::NewLine
	Set-Content "$TMP_TESTFILES_PATH\FuncGuards.h" $HeaderContent_ThreeValidAllEnabled
	Set-Content "$TMP_TESTFILES_PATH\FuncGuards_skel.h" $HeaderContent_Empty
	Set-Content "$TMP_TESTFILES_PATH\templates\FuncGuards1_template.h" $HeaderContent_SingleEnabled
	Set-Content "$TMP_TESTFILES_PATH\\templates\FuncGuards2_template.h" $HeaderContent_FourValidAllEnabled
	Set-Content "$TMP_TESTFILES_PATH\\templates\FuncGuards3_template.h" $HeaderContent_Empty

	#Act
	Update-FuncGuardFiles -All -wa 0
	[string]$skelContent = Get-Content $TMP_TESTFILES_PATH\FuncGuards_skel.h
	[string]$T1Content = Get-Content $TMP_TESTFILES_PATH\templates\FuncGuards1_template.h
	[string]$T2Content = Get-Content $TMP_TESTFILES_PATH\\templates\FuncGuards2_template.h
	[string]$T3Content = Get-Content $TMP_TESTFILES_PATH\templates\FuncGuards3_template.h

	$Actual = (
		((Compare-Object $skelContent $HeaderContent_ThreeValidAllDisabled) -eq $null) -and
		((Compare-Object $T1Content $HeaderContent_ThreeValidSingleEnabled) -eq $null) -and
		((Compare-Object $T2Content $HeaderContent_ThreeValidAllEnabled) -eq $null) -and
		((Compare-Object $T3Content $HeaderContent_ThreeValidAllDisabled) -eq $null) )

	#Assert
	Assert-That -ActualValue $Actual -Constraint {$Actual -eq $true}
}
