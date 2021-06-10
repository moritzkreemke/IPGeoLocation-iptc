
set(CMAKE_C_STANDARD 99)

FILE(COPY ../resources/asn-country-ipv4.csv DESTINATION resources)
FILE(COPY ../data/asn-country-ipv4.csv DESTINATION data)

add_executable(ConvertIPV4TpNumberTest IPV4ToNumberConversionTest.c)
add_test(ConvertIPV4TpNumberTest ConvertIPV4TpNumberTest)

add_executable(CountLinesTest CountLinesTest.c)
add_test(CountLinesTest CountLinesTest)

add_executable(ParseIPFileTest ParseIPFileTest.c)
add_test(ParseIPFileTest ParseIPFileTest)

add_executable(GetCountryFromIPTest GetCountryFromIPTest.c)
add_test(GetCountryFromIPTest GetCountryFromIPTest)

add_executable(ReadIPV4AddressFromStreamTest ReadIPV4AddressFromStreamTest.c)
add_test(ReadIPV4AddressFromStreamTest ReadIPV4AddressFromStreamTest)

add_executable(NumberToIPV4ConversionTest NumberToIPV4ConversionTest.c)
add_test(NumberToIPV4ConversionTest NumberToIPV4ConversionTest)

add_executable(IsIPTest IsIPTest.c)
add_test(IsIPTest IsIPTest)

get_filename_component(PARENT_DIR ../ ABSOLUTE)
include_directories(${PARENT_DIR}/)