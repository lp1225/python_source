#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>


//FILE *my_decrypt(char *str, char *path_key); // 解密

FILE *my_decrypt(FILE *fp, char *str, char *path_key)
{
    char *p_de = NULL;
    RSA *p_rsa = NULL;
    FILE *file = NULL;
    // long flen;

    printf("这里是测试  \n");
    if(fp == stdin){
        printf("fp是stdin中则直接返回 \n");
        return fp;

    }else{

        // // 解析文件流
        // // 文件定位
        // fseek(fp, 0L, SEEK_END);
        // flen = ftell(fp);
        // str = (char *)malloc(flen+1);

        // if(str !=NULL)
        // {
        //     fseek(fp, 0L, SEEK_SET);
        //     fread(str, flen, 1, fp);
        //     fclose(fp);
        // }
    

        // 打开秘钥文件
        file = fopen(path_key, "rb");
        if(!file){
            printf("fopen() error \n");
            goto End;
        }
        // 从私钥获取解密的秘钥
        if((p_rsa = PEM_read_RSAPrivateKey(file, NULL, NULL, NULL)) == NULL)
        {
            printf("使用私有的");
            ERR_print_errors_fp(stdout);
            goto End;
        }

        p_de = (char*)malloc(256);
        if(!p_de){
            perror("malloc() error");
            goto End;
        }

        //内存块清零
        memset(p_de, 0, 256);

        //    printf("str is %s \n", str);
        // 对内容进行解密
        if(RSA_private_decrypt(256, (unsigned char*)str, (unsigned char*)p_de, p_rsa, RSA_PKCS1_PADDING) <0)
        {
            //perror("RSA_public_encrypt() error");
            // 文件未加密，恢复文件指针到初始
            fseek(fp, 0L, SEEK_SET);
            printf("文件解密失败\n");
            // return fp;
            goto End;
        }

        printf("解析出来的p_de is %s \n", p_de);
        int length;
        length = strlen(p_de);

        fp = fmemopen(p_de, length, "r");

        End:
        // 释放秘钥空间，关闭文件
        if(p_rsa) RSA_free(p_rsa);
        if(file) fclose(file);
        if(str) free(str);
        
        printf("这里是crypt的最后一行\n");
        return fp;
    }

}
