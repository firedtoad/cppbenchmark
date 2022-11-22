add_executable(openssl_rc5_aes openssl/rc5_aes.cpp)
target_link_libraries(openssl_rc5_aes PRIVATE crypto dl)

add_executable(crypto_rc5_aes cryptopp/rc5_aes.cpp)
target_link_libraries(crypto_rc5_aes PRIVATE cryptopp dl)