# ��Ŀ�ṹ˵��
����Ŀ���� CMake ������������������Զ�����־ģ�� `myLog`��<br>
�����Ǿ�������ĿĿ¼�ṹ��
### Ŀ¼�ṹ
```text
.
������ CMakeLists.txt              # ��Ŀ����������
������ README.md                   # ��Ŀ˵���ĵ�
������ myLog/
��   ������ CMakeLists.txt          # ��־ģ�鹹������
��   ������ include/
��   ��   ������ myLogApi.h          # ��־�����ӿ�ͷ�ļ�
��   ������ src/
��       ������ myLog.cpp           # ��־ģ��ʵ��
��       ������ myLog.h             # ��־ģ��ͷ�ļ�
��       ������ myLogApi.cpp        # ��־��ӿ�ʵ��
������ lib/
��   ������ Debug_Windows_MSVC_v1900_x64/
��   ��   ������ myLogd.lib          # Debugģʽ(MSVC)���ɵľ�̬��
��   ������ Release_Windows_MSVC_v1900_x64/
��       ������ myLog.lib           # Releaseģʽ(MSVC)���ɵľ�̬��
������ bin/
��   ������ Debug_Windows_MSVC_v1900_x64/
��   ��   ������ cpp-basekit.exe     # Debugģʽ(MSVC)���ɵĿ�ִ���ļ�
��   ������ Release_Windows_MSVC_v1900_x64/
��       ������ cpp-basekit.exe     # Releaseģʽ(MSVC)���ɵĿ�ִ���ļ�
������ src/
��   ������ main.cpp                # ���������
```
### ��л
��Ŀʹ�õ��� [CLion](https://www.jetbrains.com/clion/) ������ԣ���л JetBrains �ṩ���ǿ����õ� IDE��Ҳ��л���п�Դ���ߺͿ�����������֧�֣�
### ����
��ӭ����Ȥ�������� Issue���ύ PR ��һ�����������Ŀ��