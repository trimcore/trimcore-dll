#ifndef TRIMCORE_DLL_LICENSING_H
#define TRIMCORE_DLL_LICENSING_H

namespace TRIMCORE {
    namespace Implementation {

        // CertLoad
        //  - opens 'filename', verifies header, and uses 'CertAdd' to add contained certificate
        //  - returns whether the operation succeeded
        //
        TRIMCORE_DLL_IMPORT bool TRIMCORE_APIENTRY CertLoad (const wchar_t * filename) noexcept;

        // CertAdd
        //  - 
        //  - parameters:
        //     - public_key - 32 bytes, 
        //     - signature - 64 bytes, for the 'public_key', signed by already known certificate
        //  - returns true if successfully added
        //
        TRIMCORE_DLL_IMPORT bool TRIMCORE_APIENTRY CertAdd (const void * public_key, const std::uint8_t * signature) noexcept;
        
        // CertGen
        //  - generates new public/secret key pair, signs it by existing one, and generates certificate file content
        //  - parameters:
        //     - new_secret_key - writes 32 bytes of new secret key
        //     - buffer - writes 120 bytes of a full new certificate file content
        //     - length - size of buffer; if less than 120, the function fails
        //     - signing_secret_key - 32 bytes of existing secret key, used to sign the public key stored into the new certificate
        //  - returns: number of bytes written to 'buffer' (120) or 0 on failure
        //
        TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY CertGen (std::uint8_t * new_secret_key, std::uint8_t * buffer, std::size_t length,
                                                                   const std::uint8_t * signing_secret_key) noexcept;

        TRIMCORE_DLL_IMPORT void * TRIMCORE_APIENTRY LicOpen (const wchar_t * filename) noexcept;
        TRIMCORE_DLL_IMPORT void * TRIMCORE_APIENTRY LicLoad (const void * data, std::size_t size) noexcept;
        TRIMCORE_DLL_IMPORT bool TRIMCORE_APIENTRY LicQuery (void *, void *, const wchar_t * string, bool (CALLBACK *) (void *, const wchar_t **)) noexcept;
        TRIMCORE_DLL_IMPORT void TRIMCORE_APIENTRY LicClose (void *) noexcept;

        // LicWrite
        //  - 
        //  - parameters:
        //     - 
        //     - data - must be UTF-16
        //     - data_length - length of 'data' in wchar_t's
        //
        TRIMCORE_DLL_IMPORT std::size_t TRIMCORE_APIENTRY LicWrite (void * buffer, std::size_t length,
                                                                    const wchar_t * data, std::size_t data_length,
                                                                    const std::uint8_t * signing_secret_key) noexcept;
    }

    // void LoadCertificate (filename);
    // void AddCertificate (GUID, public key, signature);

    // HANDLE OpenLicense (const wchar_t *);
    // HANDLE OpenLicense (const void *, std::size_t);
    // std::vector<std::wstring_view> GetLicense (HANDLE, L"something{nested{prefix*");
    // void CloseLicense ();

}

#endif
