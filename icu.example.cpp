
#include <iostream>
#include <fstream>
#include <unicode/ustream.h>
#include <unicode/unistr.h>
#include <unicode/ucnv.h>
#include <vector>

void icu_utf8()
{
  // Input Unicode string
  std::u16string unicodeString = u"unicode based on machine locale setting (한글)";

  // Convert Unicode to UTF-8
  UErrorCode err = U_ZERO_ERROR;
  UConverter *conv = ucnv_open("UTF-8", &err);

  if (U_SUCCESS(err))
  {
    std::string utf8String;
    icu::UnicodeString(unicodeString.c_str()).toUTF8String(utf8String);

    // Now utf8String contains the UTF-8 representation of the Unicode string
    std::cout << "UTF-8: " << utf8String << std::endl;

    ucnv_close(conv);
  }

  return;
}

void icu_euc_kr()
{
  // Unicode string
  icu::UnicodeString unicodeString = u"안녕하세요"; // Korean: "Hello"

  // Convert Unicode to EUC-KR
  UErrorCode err = U_ZERO_ERROR;
  UConverter *conv = ucnv_open("EUC-KR", &err);
  std::cout << "err: " << err << std::endl;
  if (U_SUCCESS(err))
  {
    int32_t targetLength = 0;
    char *target = nullptr;

    // Pre-flight to determine the length of the target buffer
    targetLength = ucnv_fromUChars(conv, nullptr, 0, unicodeString.getBuffer(), unicodeString.length(), &err);
    std::cout << "err: " << err << std::endl;
    std::cout << "targetLength: " << targetLength << std::endl;
    if (err == U_BUFFER_OVERFLOW_ERROR)
    {
      err = U_ZERO_ERROR;
      if (U_SUCCESS(err))
      {
        target = new char[targetLength + 1];
        ucnv_fromUChars(conv, target, targetLength + 1, unicodeString.getBuffer(), unicodeString.length(), &err);
        if (U_SUCCESS(err))
        {
          const std::string filepath = "output.txt";
          std::ofstream outfile(filepath, std::ios::binary);
          if (outfile.is_open())
          {
            outfile.write(target, targetLength);
            outfile.close();
            std::cout << "Bytes saved to file: " << filepath << std::endl;
          }
          else
          {
            std::cerr << "Failed to open the file for writing." << std::endl;
          }

          // Now, 'target' contains the EUC-KR representation of the Unicode string
          std::cout << "EUC-KR: " << target << std::endl;
        }
      }
    }

    delete[] target;
    ucnv_close(conv);
  }

  return;
}


// void icu_euc_jp()
// {

//   // Unicode string
//   icu::UnicodeString unicodeString = L"は゛ば"; // Korean: "Hello"

//   // Convert Unicode to EUC-KR
//   UErrorCode err = U_ZERO_ERROR;
//   UConverter *conv = ucnv_open("EUC-KR", &err);
//   std::cout << "err: " << err << std::endl;
//   if (U_SUCCESS(err))
//   {
//     int32_t targetLength = 0;
//     char *target = nullptr;

//     // Pre-flight to determine the length of the target buffer
//     targetLength = ucnv_fromUChars(conv, nullptr, 0, unicodeString.getBuffer(), unicodeString.length(), &err);
//     std::cout << "err: " << err << std::endl;
//     std::cout << "targetLength: " << targetLength << std::endl;
//     if (err == U_BUFFER_OVERFLOW_ERROR)
//     {
//       err = U_ZERO_ERROR;
//       if (U_SUCCESS(err))
//       {
//         target = new char[targetLength + 1];
//         ucnv_fromUChars(conv, target, targetLength + 1, unicodeString.getBuffer(), unicodeString.length(), &err);
//         if (U_SUCCESS(err))
//         {
//           const std::string filepath = "output.txt";
//           std::ofstream outfile(filepath, std::ios::binary);
//           if (outfile.is_open())
//           {
//             outfile.write(target, targetLength);
//             outfile.close();
//             std::cout << "Bytes saved to file: " << filepath << std::endl;
//           }
//           else
//           {
//             std::cerr << "Failed to open the file for writing." << std::endl;
//           }

//           // Now, 'target' contains the EUC-KR representation of the Unicode string
//           std::cout << "EUC-KR: " << target << std::endl;
//         }
//       }
//     }

//     delete[] target;
//     ucnv_close(conv);
//   }

//   return;
// }