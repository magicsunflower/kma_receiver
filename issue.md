## 开发问题汇总

- 端点发送失败，但是字段重复发出，usb_endp.c中配置端点与实际不匹配导致，全部配置即可正常发送数据（把下面的注释打开）
  SetEPType(ENDP3, EP_INTERRUPT);
  SetEPTxAddr(ENDP3, ENDP3_TXADDR);
  SetEPTxStatus(ENDP3, EP_TX_NAK);
  _ClearDTOG_TX(ENDP3);
  _ClearDTOG_RX(ENDP3);

  // SetEPType(ENDP4, EP_INTERRUPT);
  // SetEPTxAddr(ENDP4, ENDP4_TXADDR);
  // SetEPTxStatus(ENDP4, EP_TX_NAK);
  // _ClearDTOG_TX(ENDP4);
  // _ClearDTOG_RX(ENDP4);
- 大数据接收丢包

  接收数据在PC端处理，模块不做处理，直接保存
- 关机重启后usb不识别

  未知，核了一下原始代码，主要更新hw_confg.c的Enter_LowPowerMode与Leave_LowPowerMode方法
- 无法写入flash数据

  flash没有初始化
- 官方博客：https://www.cnblogs.com/wchmcu/p/
- 官方教程：https://bbs.21ic.com/icview-3037572-1-1.html
- 鼠标移动数据发送示例：

  ```c
   uint8_t MS_Relative_Data_Pack[DEF_ENDP_SIZE_RELATIVE_MS] = {0x00}; // Mouse IN Data Packet
      uint8_t MS_Absolute_Data_Pack[DEF_ENDP_SIZE_ABSOLUTE_MS] = {0x00};
      MS_Relative_Data_Pack[1] = 5; //x
      MS_Relative_Data_Pack[2] = 5; // y

      // 分辨率2560x1440移动到640x480:
      // 对于X轴：640 * (32767 / 2560) = 8191(1FFF)
  　　// 对于Y轴：480 * (32767 / 1440) = 10922(2AAA)
      MS_Absolute_Data_Pack[0] = 0x00; // x low
      MS_Absolute_Data_Pack[1] = 0xFF; // x low
      MS_Absolute_Data_Pack[2] = 0x1F; // x high
      MS_Absolute_Data_Pack[3] = 0xAA; // y low
      MS_Absolute_Data_Pack[4] = 0x2A; // y high
      int count = 10;
      while(1 && count--){
          send_hid_event(RELATIVE_MOUSE,MS_Relative_Data_Pack,DEF_ENDP_SIZE_RELATIVE_MS);
          Delay_Ms(500);
          if(count == 1){
               send_hid_event(ABSOLUTE_MOUSE,MS_Absolute_Data_Pack,DEF_ENDP_SIZE_ABSOLUTE_MS);
          }
      }
  ```
