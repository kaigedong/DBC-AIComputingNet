/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __USBD_LIB_MSC_H__
#define __USBD_LIB_MSC_H__

#ifdef __cplusplus
extern "C"  {
#endif

/*--------------------------- USB Requests -----------------------------------*/

extern void USBD_MSC_ClrStallEP(U32 EPNum);
extern BOOL USBD_MSC_Reset     (void);
extern BOOL USBD_MSC_GetMaxLUN (void);
extern void USBD_MSC_GetCBW    (void);
extern void USBD_MSC_SetCSW    (void);

#ifdef __cplusplus
}
#endif

#endif  /* __USBD_LIB_MSC_H__ */
