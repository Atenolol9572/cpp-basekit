# ��Ŀ�ṹ˵��
����Ŀ���� CMake ������������������Զ�����־ģ�� `myLog`��<br>
�����Ǿ�������ĿĿ¼�ṹ��
### Ŀ¼�ṹ
```text
.
������ CMakeLists.txt              # ��Ŀ����������
������ README.md                   # ��Ŀ˵���ĵ�
������ include/
��   ������ myLogLib.h              # ��־�����ӿ�ͷ�ļ�
������ lib/
��   ������ Debug/
��   ��   ������ myLogLib_debug.lib  # Debug ģʽ���ɵľ�̬��
��   ������ Release/
��       ������ myLogLib.lib        # Release ģʽ���ɵľ�̬��
������ bin/
��   ������ Debug/
��   ��   ������ test_github.exe     # Debug ģʽ���ɵĿ�ִ���ļ�
��   ������ Release/
��       ������ test_github.exe     # Release ģʽ���ɵĿ�ִ���ļ�
������ myLog/
��   ������ CMakeLists.txt          # ��־ģ�鹹������
��   ������ src/
��       ������ myLog.cpp           # ��־ģ��ʵ��
��       ������ myLog.h             # ��־ģ��ͷ�ļ�
��       ������ myLogLib.cpp        # ��־��ӿ�ʵ��
������ src/
��   ������ main.cpp                # ���������
```
### ��л
��Ŀʹ�õ��� [CLion](https://www.jetbrains.com/clion/) ������ԣ���л JetBrains �ṩ���ǿ����õ� IDE��Ҳ��л���п�Դ���ߺͿ�����������֧�֣�
### ����
��ӭ����Ȥ�������� Issue���ύ PR ��һ�����������Ŀ��