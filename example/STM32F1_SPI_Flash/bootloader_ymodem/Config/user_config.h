/**
 * \file            user_config.h
 * \brief           user configuration of the application
 */

/*
 * Copyright (c) 2022 Dino Haw
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of mOTA - The Over-The-Air technology component for MCU.
 *
 * Author:          Dino Haw <347341799@qq.com>
 * Version:         v1.0.3
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-23     Dino         the first version
 * 2022-12-04     Dino         1. ����һ����¼�汾�Ļ��ƣ���ѡд�� APP ����
 *                             2. ���ӳ��������ָ������̼���ѡ��
 *                             3. �� flash �Ĳ��������������� user_config.h 
 *                             4. �����Ƿ��жϹ̼�������������С��ѡ��
 * 2022-12-07     Dino         ���� ONCHIP_FLASH_ONCE_WRITE_BYTE ������
 * 2022-12-08     Dino         1. ���ӹ̼����ɷ����� SPI flash �Ĺ���
 *                             2. ���� ENABLE_FACTORY_UPDATE_TO_APP ������
 */

/**
 * OTA����Ŀ�������ѡ��
 */
#include "app_config.h"

/**
 * ��ѡ�����������
 * ����: 
 *    APP:      �����еĹ̼�����
 *    download: ���ڸ��¹̼�ʱ�Ĺ̼���ʱ�������
 *    factory:  ���ڴ�ſ��ڽ�������»ָ��̼�ʹ�õ�����
 *    ONE_PART_PROJECT:     ������������ APP ��
 *    DOUBLE_PART_PROJECT:  ˫���������� APP + download ��
 *    TRIPLE_PART_PROJECT:  ������������ APP + download + factory ��
 * ѡ��: 
 *    ONE_PART_PROJECT      �� 0
 *    DOUBLE_PART_PROJECT   �� 1
 *    TRIPLE_PART_PROJECT   �� 2
 */
#define USING_PART_PROJECT                  TRIPLE_PART_PROJECT


/**
 * ��flash��������
 * ˵��: 
 *    ���ø��������Ĵ�С
 * ע������: 
 *    ������Ƭ�� Flash �����ҳ���룬 �����׵�ַ������ Flash �� ÿ������ page �� sector ���׵�ַ������̼��޷����У�����
 *    ������������ SPI Flash �ķ���������Ҫ��С��д���ȵ�������Ϊ��λ���ж��룬������ sector ��������Ϊ��λ���ж��룡����
 */
#define ONCHIP_FLASH_SIZE                   ((uint32_t)(512 * 1024))    /* Ƭ�� flash ��������λ: byte */
#define BOOTLOADER_SIZE                     ((uint32_t)(32 * 1024))     /* Ԥ���� bootloader �Ŀռ䣬��λ: byte����С��Ҫ���ڱ����̱����Ĵ�С�� */
#define APP_PART_SIZE                       ((uint32_t)(32 * 1024))     /* Ԥ���� APP �����Ŀռ䣬��λ: byte��ע��ҳ���룩 */
#define DOWNLOAD_PART_SIZE                  (APP_PART_SIZE)             /* Ԥ���� download �����Ŀռ䣬��λ: byte��ע��ҳ���룬��ʹ��ʱ��д0�� */
#define FACTORY_PART_SIZE                   (APP_PART_SIZE)             /* Ԥ���� factory �����Ŀռ䣬��λ: byte��ע��ҳ���룬��ʹ��ʱ��д0�� */


/**
 * ��ѡ���Ƿ����ý��������
 * ˵��: 
 *    ���̼����м��ܣ���������á����̼����޼��ܣ��ɰ���ѡ���Ƿ�����
 *    * AES256_KEY ������� 32 �ֽڣ� AES256_IV ������� 16 �ֽ�
 * ѡ��: 
 *    0: ������
 *    1: ����
 */
#define ENABLE_DECRYPT                      1
    #if (ENABLE_DECRYPT)
    #define AES256_KEY                      "0123456789ABCDEF0123456789ABCDEF"  /* ������� 32 �ֽ� */
    #define AES256_IV                       "0123456789ABCDEF"                  /* ������� 16 �ֽ� */
    #endif


/**
 * ��Ƭ�� flash һ��д�����С�ֽ�����
 * ˵����
 *    ÿ�� MCU �ڲ� flash ���ο���д����ֽ���������ͬ������ṩ��ѡ���������ã���λ�� byte
 *    ��ֵ�Ĵ�С���ڲ� flash �Ĳ����ӿھ�����Ĭ���� 4 byte �������������ã���Ҫ�����ѡ����ζ��ʲô
 */
#define ONCHIP_FLASH_ONCE_WRITE_BYTE        4


/**
 * ��ѡ�� bootloader ����Ƿ���Ҫ�̼����µķ�����
 * ˵����
 *    ���豸�ϵ�ʱ�������� bootloader ����� bootloader ��Ҫ��һ����ȡ�Ƿ���Ҫ���й̼����µķ���
 * ���ͣ�
 *    USING_HOST_CMD_UPDATE:     ͨ������ָ����ƽ���̼�����ģʽ���˷�ʽ��ȴ�һ��ʱ�䣬��ȷ���Ƿ���Ҫ���й̼����£�
 *                               �ȴ�ʱ��ͨ�� WAIT_HOST_DATA_MAX_TIME ��������
 *    USING_APP_SET_FLAG_UPDATE: ͨ���� APP �����ñ�־λ�� bootloader ����ʱ��ͨ����ȡ�ñ�־λ���ж���Ҫ���й̼�����
 *                               ���������̼�����
 * ѡ�
 *    USING_HOST_CMD_UPDATE      �� 0
 *    USING_APP_SET_FLAG_UPDATE  �� 1
 */
#define USING_IS_NEED_UPDATE_PROJECT        USING_APP_SET_FLAG_UPDATE
    #if (USING_IS_NEED_UPDATE_PROJECT == USING_HOST_CMD_UPDATE)
    #define WAIT_HOST_DATA_MAX_TIME         (3 * 1000)      /* ���õȴ��������ݵ����ȴ�ʱ�䣬��λ ms */
    #else
    #define WAIT_HOST_DATA_MAX_TIME         (60 * 1000)     /* ���õȴ��������ݵ����ȴ�ʱ�䣬��λ ms
                                                             * �������ʱ��û���յ�������ָ���ζ���������ܵ��ߣ�
                                                             * ��ʱ bootloader �᳢����ת�� APP
                                                             * ����Ϊ 0 �����ʾ�����������ߣ�Ҳ��������ת�� APP
                                                             */
    #define FIRMWARE_UPDATE_VAR_ADDR        0x20000000      /* �̼����±�־λ�Ĵ�ŵ�ַ���������޸ģ�һ��Ҫ�� APP һ�£� */
    #define FIRMWARE_UPDATE_MAGIC_WORD      0xA5A5A5A5      /* �̼���Ҫ���µ������ǣ��������޸ģ�һ��Ҫ�� APP һ�£� */
    #define FIRMWARE_RECOVERY_MAGIC_WORD    0x5A5A5A5A      /* ��Ҫ�ָ������̼��������ǣ��������޸ģ�һ��Ҫ�� APP һ�£� */
    #define BOOTLOADER_RESET_MAGIC_WORD     0xAAAAAAAA      /* bootloader ��λ�������ǣ��������޸ģ�һ��Ҫ�� APP һ�£� */
    #endif


/**
 * ��ѡ���Ƿ�ʹ�ð����ָ������̼���ѡ�
 * ˵��: 
 *    1. ʹ�ñ�ѡ���ǰ������������������ѡ������Ч��ǰ������ȷ�����˰����� factory �����п��õĹ̼�
 *    2. ѡ��ʹ�ð����ָ������̼�ʱ����Ҫ���ð��������ţ���ʹ�õİ����ͱ�������ͬ������Ҫ�Լ����úͳ�ʼ�� GPIO
 *    3. ��ѡ����豸������ bootloader ʱ����ͨ�������ָ������̼������豸������ APP ����ѡ�����޷���Ч�ģ������Ҫ APP Ҳͬ������
 *    4. ��ѡ���ͨ��ָ��ָ������̼��ķ�ʽ����ͻ������ͬʱʹ�ã�Ҳ���Բ����ñ�ѡ��
 *    5. ���� factory �� factory �޿��ù̼�ʱ��ǿ�лָ������̼������ᴥ�� FACTORY_NO_FIRMWARE_SOLUTION ѡ��
 * ���ͣ�
 *    ENABLE_FACTORY_FIRMWARE_BUTTON: ѡ���Ƿ����ó��������ָ������̼�
 *    FACTORY_FIRMWARE_BUTTON_PRESS:  ��������ʱ�ĵ�ƽ�߼�
 *    FACTORY_FIRMWARE_BUTTON_TIME:   ���������ĳ���ʱ�䣬��λ ms
 * ѡ�
 *    ENABLE_FACTORY_FIRMWARE_BUTTON ѡ��: 
 *        0: ������
 *        1: ����
 *    FACTORY_FIRMWARE_BUTTON_PRESS ѡ��: 
 *        KEY_PRESS_LOW:  ��ʾ����ʱΪ�͵�ƽ
 *        KEY_PRESS_HIGH: ��ʾ����ʱΪ�ߵ�ƽ
 *    FACTORY_FIRMWARE_BUTTON_TIME ѡ��: 
 *        ���������ĳ���ʱ�䣬��λ ms �����ܴ��� 65535
 */ 
#if (USING_PART_PROJECT == TRIPLE_PART_PROJECT)
#define ENABLE_FACTORY_FIRMWARE_BUTTON      0
#define FACTORY_FIRMWARE_BUTTON_PRESS       KEY_PRESS_LOW
#define FACTORY_FIRMWARE_BUTTON_TIME        3000
#endif


/**
 * ��ѡ��ͨ�� bootloader ���̼��������� factory ����ʱ�Ƿ������ APP ��ѡ�
 * ˵��: 
 *    ͨ�� bootloader ���̼��������� factory ����ʱ�Ƿ��Զ������� APP ���������ã��� bootloader ���ڹ̼��������� factory 
 *    ������У��ɹ�������ת�� APP �������� APP �����޿��õĹ̼���У��ʧ�ܣ�������� FACTORY_NO_FIRMWARE_SOLUTION ѡ��
 *    ִ�ж�Ӧ�ķ���
 * ѡ��: 
 *    0: ������
 *    1: ����
 */ 
#define ENABLE_FACTORY_UPDATE_TO_APP        0


/**
 * ��ѡ���жϹ̼����Ƿ񳬹�������С��ѡ�
 * ˵��: 
 *    �������ж�ʱ���������⴦���������޸Ĵ��룬λ�� app.c �� APP_Running ���� �� EXE_FLOW_VERIFY_FIRMWARE_HEAD ����
 * ѡ��: 
 *    0: ������
 *    1: ����
 */ 
#define ENABLE_CHECK_FIRMWARE_SIZE          1


/**
 * ��ѡ���Զ����¹̼��Ĵ�������
 * ˵��: 
 *    1. �ڹ̼����¹������豸�쳣�ϵ��������ѡ���Ƿ��Զ����������غõĹ̼��Լ��Զ����µĴ�����
 *    2. ��ѡ���ִ�����ȼ�������λ�����µķ�ʽ������ζ�ų�����λ����ʱδ�������ݣ����򽫻�����ִ����λ���Ĺ̼�����
 * ����: 
 *    
 *    DO_NOT_AUTO_UPDATE:           ����Ҫ�Զ����£���ϣ�������ֶϵ�ָ��̼��Ļ���
 *    ERASE_DOWNLOAD_PART_PROJECT:  ������ɺ���� download �������豸�ϵ�ʱ��ͨ����� download �������޿��ù̼�
 *                                  ���ж��Ƿ���Ҫ�Զ����¹̼�
 *                                  * ѡ��˷����󣬽��޷�ѡ���Ƿ����ϵ��� APP �Ĺ̼����а���У��( USING_APP_SAFETY_CHECK_PROJECT )��
 *                                    ��Ϊ APP �̼�����У���������Դ�� download �����Ĺ̼�����
 *    MODIFY_DOWNLOAD_PART_PROJECT: ������ɺ��޸� download �����Ĺ̼���ͷ�İ汾��Ϣ���豸�ϵ�ʱ��Ա� download �����̼���ͷ��¼���¾ɰ汾��
 *                                  ���¾ɰ汾��һ�£���ʼ�Զ����¹̼�
 *                                  * ���ַ�ʽ��Ҫ�޸� download ���������ݣ������������ƣ�
 *                                    1. ���ƣ��ϵ�ʱ��У�� APP �����Ĺ̼�������ȷ�Ժ������ԣ������ APP �̼����𻵻���۸�ʱ�İ�ȫ�ԣ�����
 *                                             ���Խ��̼��ָ���������Ч���ϵͳ�İ�ȫ�ȼ�
 *                                    2. ���ƣ���Ҫ�Ա�ͷ���ڵ� flash sector ������������д�룬����ζ��ÿ�θ��¶������ͬ�� sector ���Ρ�
 *                                             ���� flash ����С�������ȱȽϴ󣬸÷�ʽ���޷�ʵ�ֵģ���Ϊ���� MCU �� flash ��С��������
 *                                             �ϴ��� STM32 �� F4 ϵ�У��� sector0 �� 16 byte ������� VERSION_WRITE_TO_APP ѡ�
 *                                             ʵ�ڵò���ʧ������ sector ��С���� 1 �� 2 kByte �������Ƽ�ѡ�� VERSION_WRITE_TO_APP                                   
 *    VERSION_WRITE_TO_APP:         ������ɺ��µĹ̼��汾д�� APP ������β����ռ�� 16 byte ���豸�ϵ�ʱ��Ա� download �����̼���ͷ
 *                                  ��¼�İ汾�� APP ��ŵİ汾���������汾��һ�£���ʼ�Զ����¹̼�
 *                                  * ���ַ�ʽ�����������ƣ�
 *                                    1. ���ƣ��ϵ�ʱ��У�� APP �����Ĺ̼�������ȷ�Ժ������ԣ������ APP �̼����𻵻���۸�ʱ�İ�ȫ�ԣ�����
 *                                             ���Խ��̼��ָ���������Ч���ϵͳ�İ�ȫ�ȼ�
 *                                    2. ���ƣ���Ҫռ�� APP ����β�� 16 byte �Ŀռ�
 * ѡ��: 
 *    DO_NOT_AUTO_UPDATE            �� 0
 *    ERASE_DOWNLOAD_PART_PROJECT   �� 1
 *    MODIFY_DOWNLOAD_PART_PROJECT  �� 2
 *    VERSION_WRITE_TO_APP          �� 3
 */
#if (USING_PART_PROJECT > ONE_PART_PROJECT)
#define USING_AUTO_UPDATE_PROJECT           VERSION_WRITE_TO_APP
#endif


/**
 * ��Ƭ�� Flash ���ù̼������� sector �Ĳ������ȡ�
 * ˵��: 
 *    USING_AUTO_UPDATE_PROJECT = MODIFY_DOWNLOAD_PART_PROJECT ʱ����Ҫ�����̼������� sector �Ĳ������ȣ���λ�� byte
 */
#if (USING_AUTO_UPDATE_PROJECT == MODIFY_DOWNLOAD_PART_PROJECT)
#define ONCHIP_FLASH_ERASE_GRANULARITY      FLASH_PAGE_SIZE
#endif


/**
 * ��ѡ���Ƿ����ϵ��� APP �Ĺ̼����а���У�� �� APP �̼����������ʱ�Ĳ���������
 * ˵��: 
 *    1. USING_AUTO_UPDATE_PROJECT = MODIFY_DOWNLOAD_PART_PROJECT ʱ�������òŻ���Ч
 *    2. ���ֲ�Ʒ�Թ̼����������а���ȼ�Ҫ�󣬱����֧�� APP �̼������������Լ�飬ͨ��������ѡ���Ƿ�����
 *    3. ������ʱ��ͨ������ USING_APP_SAFETY_PROJECT ��ѡ�� APP �̼����������ʱ�Ĳ�������
 * ����: 
 *    DO_NOT_CHECK      : ��У�� APP �̼�����������
 *    CHECK_UNLESS_EMPTY: У�� APP �̼������޷�У��ʱ��У�飬ȷ�������� APP �������ڵ��� bootloader �С��� APP �̼�У����󣬽���
 *                        �Զ��ѿ��ú���ȷ�Ĺ̼������� APP 
 *                        * �޷����� APP �̼�У�������ǣ� download ������ factory �������޿��ù̼���
 *    AUTO_UPDATE_APP   : APP �̼�У�������Զ������ú���ȷ�Ĺ̼������� APP
 *                        * ��Ҫע����ǣ���ѡ���˱�ѡ���ζ����ʮ������ APP �����������ԣ�Ҳ����˵���� APP �̼�У�鲻ͨ����
 *                          �޷�У��ʱ���� download ������ factory �������޿��ù̼��������ͣ���� bootloader �У�������ת�� APP ��
 *                          ���� APP ���ڹ̼�
 *                        * ��Ҫ�ر��������ǣ���һ������ᵼ�� APP �޷���ִ�У��Ǳ���ͨ����¼�����̼���¼�� MCU �� flash �У�
 *                          ��Ϊ��ʱ����ͨ�������Ĺ̼����³���ִ�У� download ������ factory �������޿��ù̼����� APP �̼��޷�����
 *                          ������У�飬������������Ĺ̼��������̣����� bootloader ����̼����£���ѡ�� CHECK_UNLESS_EMPTY ѡ��
 *                        * �˴����Զ����º� USING_AUTO_UPDATE_PROJECT �Ĳ�ͬ����ѡ����� APP �̼�У�鲻ͨ��ʱ�Ż�
 *                          �Զ����£��� USING_AUTO_UPDATE_PROJECT �������ӱ�ѡ����й̼��Զ����� 
 *    DO_NOT_DO_ANYTHING: APP�̼�У������Ҫ���κβ�����ͣ�� bootloader ���ɣ����� download ������ factory �����п��õĹ̼���
 *                        * ��Ҫע����ǣ� DO_NOT_DO_ANYTHING ���ѡ�������ֹ APP ����Ϊ��ʱ�� USING_AUTO_UPDATE_PROJECT ������
 *                          �Զ����µ������ DO_NOT_DO_ANYTHING ֻ������APP������Ϊ����У�鲻ͨ���������Ҫ��ֹ�Զ����£���Ҫ�޸�
 *                          �Ϸ��� USING_AUTO_UPDATE_PROJECT ѡ��Ϊ DO_NOT_AUTO_UPDATE
 * ѡ��:  USING_APP_SAFETY_PROJECT
 *    DO_NOT_CHECK          �� 0
 *    CHECK_UNLESS_EMPTY    �� 1
 *    AUTO_UPDATE_APP       �� 2
 *    DO_NOT_DO_ANYTHING    �� 3
 */
#if (USING_PART_PROJECT > ONE_PART_PROJECT &&   \
     (USING_AUTO_UPDATE_PROJECT == MODIFY_DOWNLOAD_PART_PROJECT || USING_AUTO_UPDATE_PROJECT == VERSION_WRITE_TO_APP))
#define USING_APP_SAFETY_CHECK_PROJECT      CHECK_UNLESS_EMPTY
#endif


/**
 * ��ѡ���Ƿ����ʹ�� factory �����Ĺ̼�����
 * ˵��: 
 *    �������Զ����»�У�� APP �̼�������ʱ���� APP �̼������ã��� download ����û�п��õĹ̼�ʱ�������� factory ������
 *    �� factory �����п��õĹ̼���ѡ���Ƿ� factory �Ĺ̼������� APP ��
 * ѡ��: 
 *    0: ��ʹ��
 *    1: ʹ��
 */
#if (USING_PART_PROJECT == TRIPLE_PART_PROJECT && \
     (USING_APP_SAFETY_CHECK_PROJECT == CHECK_UNLESS_EMPTY || USING_APP_SAFETY_CHECK_PROJECT == AUTO_UPDATE_APP))
#define ENABLE_USE_FACTORY_FIRWARE          1
#endif


/**
 * ��ѡ����Ҫ�ָ������̼�ʱ���� factory �����޹̼���̼�У��������ʱ�Ľ��������
 * ���ͣ�
 *    JUMP_TO_APP:           ������ת�� APP
 *    WAIT_FOR_NEW_FIRMWARE: �ȴ���λ�������µĹ̼���
 * ѡ�
 *    JUMP_TO_APP            �� 0
 *    WAIT_FOR_NEW_FIRMWARE  �� 1
 */
#if (USING_PART_PROJECT == TRIPLE_PART_PROJECT)
#define FACTORY_NO_FIRMWARE_SOLUTION        JUMP_TO_APP          
#endif


/**
 * ��ѡ���Ƿ��Զ������̼��ķ�����
 * ˵��: 
 *    ��ѡ����Ϊ��������Ϊ�Ľ�������д����������һ�������̶ȱ�֤�̼�������������ȴ�ʩ
 *    �����������£����۱������Ƿ����ã��̼���ָ�����������������ᱻ����Ϊ APP ������ʹ�������������
 *    ����������£����ú󣬹̼���ָ��Ϊ APP ����ʱ���ᱻ����Ϊ download ������ʹ�������������
 *    ����������£��������ã��̼���ָ��Ϊ APP ����ʱ���ᱨ�������Ϊ����ʧ��
 * ѡ��: 
 *    0: ������
 *    1: ����
 */
#if (USING_PART_PROJECT > ONE_PART_PROJECT)
#define ENABLE_AUTO_CORRECT_PART             1
#endif


/**
 * ��ѡ������Ĵ��λ�á�
 * ˵��: 
 *    ��ѡ������ SPI Flash ��Ź̼�ʱ����Ҫָ�� download �� factory �����Ĵ��λ��
 *    ������ʹ���� FAL �⣬���ڹ��� flash �ķ��������������� SPI Flash ʱ��
 *    �������ݣ����:  https://github.com/RT-Thread-packages/fal
 *    ������ͬʱ��ʹ���� SFUD �⣬��˿��Զ����ݲ�ͬ���ҺͲ�ͬ������ SPI Flash 
 *    SPI Flash �ĵײ�ӿ���ֲ�ļ��� sfud_port.c ��ʵ���ڲ� API ����
 *    ��ʹ�õ� SPI Flash ��֧�� SFDP ������Ҫ�޸� sfdu_flash_def.h �ļ�
 *    �޸ġ���ֲ��ʽ�͸������ݣ����:  https://github.com/armink/SFUD
 * ����: 
 *    STORE_IN_ONCHIP_FLASH : Ƭ�� Flash
 *    STORE_IN_SPI_FLASH    : SPI Flash
 * ע������: 
 *    ��������ز���ǿ�ƽ� APP ���������� SPI Flash �ڡ����򣬹̼����޷����У�����
 * ѡ��: 
 *    STORE_IN_ONCHIP_FLASH �� 0
 *    STORE_IN_SPI_FLASH    �� 1
 */
#if (USING_PART_PROJECT > ONE_PART_PROJECT)
#define DOWNLOAD_PART_LOCATION              STORE_IN_SPI_FLASH
#if (USING_PART_PROJECT == TRIPLE_PART_PROJECT)
#define FACTORY_PART_LOCATION               STORE_IN_SPI_FLASH
#endif

/* ��Ҫ�޸� IS_ENABLE_SPI_FLASH ��ֵ */
#if (DOWNLOAD_PART_LOCATION == STORE_IN_SPI_FLASH || FACTORY_PART_LOCATION == STORE_IN_SPI_FLASH)
#define IS_ENABLE_SPI_FLASH                 1
#endif
#endif  /* #if (USING_PART_PROJECT > ONE_PART_PROJECT) */


/**
 * ��SPI Flash ���ù̼������� sector �Ĳ������ȡ�
 * ˵��: 
 *    1. ������ SPI Flash ʱ����Ҫ�����̼������� sector �Ĳ������ȣ���λ�� byte
 *    2. �� flash ֧�� SFDP ���Ѿ����� SFUD_USING_SFDP ���� SPI_FLASH_ERASE_GRANULARITY �������������⣬
 *       ��ᱻ������ SFDP ���� 
 */
#if (IS_ENABLE_SPI_FLASH)
#define SPI_FLASH_ERASE_GRANULARITY         4096
#endif




