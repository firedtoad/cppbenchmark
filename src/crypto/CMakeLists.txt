add_executable(crypto_rc5_aes cryptopp/rc5_aes.cpp )
target_link_libraries(crypto_rc5_aes PRIVATE cryptopp )

add_executable(crypto_dh cryptopp/crypto_dh.cpp)
target_link_libraries(crypto_dh PRIVATE cryptopp crypto dl)

#add_executable(openssl_rc5_aes openssl/rc5_aes.cpp)
#target_link_libraries(openssl_rc5_aes PRIVATE cryptopp crypto)

#add_executable(crypto_tps cryptopp/crypto_tps.cpp)
#target_link_libraries(crypto_tps PRIVATE cryptopp tps_client tps_server dl atomic)

#target_compile_options(openssl_rc5_aes PRIVATE -fsanitize=address)
#target_link_options(openssl_rc5_aes PRIVATE -fsanitize=address)

#target_compile_options(crypto_rc5_aes PRIVATE -fsanitize=address)
#target_link_options(crypto_rc5_aes PRIVATE -fsanitize=address)