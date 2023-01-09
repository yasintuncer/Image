// Author: Yasin Tunçer - 2023 
// QUOTED: PSD_SDK - MOLECULAR MATTERS GMBH https://github.com/MolecularMatters/psd_sdk.git

#pragma once


/// \def IMAGE_NAMESPACE_NAME
/// \ingroup Platform
/// \brief Macro used to configure the name of the Image library namespace.
#define IMAGE_NAMESPACE_NAME						Image


/// \def IMAGE_NAMESPACE
/// \ingroup Platform
/// \brief Macro used to refer to a symbol in the Image library namespace.
/// \sa IMAGE_USING_NAMESPACE
#define IMAGE_NAMESPACE							IMAGE_NAMESPACE_NAME


/// \def IMAGE_NAMESPACE_BEGIN
/// \ingroup Platform
/// \brief Macro used to open a namespace in the Image library.
/// \sa IMAGE_NAMESPACE_END
#define IMAGE_NAMESPACE_BEGIN						namespace IMAGE_NAMESPACE_NAME {


/// \def IMAGE_NAMESPACE_END
/// \ingroup Platform
/// \brief Macro used to close a namespace previously opened with \ref IMAGE_NAMESPACE_BEGIN.
/// \sa IMAGE_NAMESPACE_BEGIN
#define IMAGE_NAMESPACE_END						}


/// \def IMAGE_USING_NAMESPACE
/// \ingroup Platfrom
/// \brief Macro used to make the Image library namespace available in a translation unit.
/// \sa IMAGE_NAMESPACE
#define IMAGE_USING_NAMESPACE						using namespace IMAGE_NAMESPACE_NAME
