//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \page page01 1. ���������
    \tableofcontents

    \section sec11 1. ������������� ���������� �����.

    \section sec12 2. �������������� � �������� �����������.
    ����� ������ � �������� ����������� ���������� ����� SPI.
    ������� ������� �� ������� ������ @ref Command � ������� ������ @ref CommandGenerator.
    ������� �������� �������� ���������.
    ����� ������������� �������� ��������� �������� ��������� ����� @ref READ_SERVICE_COMMAND � �������� 100 ��� � �������. � ������, ���� ��������� 
    ������ �� ����� ������ ��� ��������, �� �� �������� ������� ����� @ref WRITE_SERVICE_COMMAND. � ������, ���� ������� ������ ��� �������� �� 
    ������ � ��������� ����������, ��� ����������. � ���� ������ �������� ��������� ����������� �������� ������ �� ���������. ����� ��������� ��������
    �������� ������������ � ��������� ��������.

    \code
    ������ ������, ������� ����� ���� ������� �� ��������� ����������.
    +----------------------+---------------------------------------------------------------+--------------------------------------------------------+
    | �������              | ������ �������                                                | ��������                                               |
    +----------------------+---------------------------------------------------------------+--------------------------------------------------------+
    | READ_SERVICE_COMMAND | COM                                                           |"������" �������, ����� ��� ����� ���������� �� ������ |
    | READ_POINTS          | COM Channel[1] �����_�����[1] �����1[1]                       |���� ����� ��� ������� ������������� �������           |
    |                      |     �����2[1] ... �����N[1]                                   |                                                        |
    +----------------------+---------------------------------------------------------------+--------------------------------------------------------+
    \endcode
    \n
    \code
    ������ ������, ������� ����� ���� �������� � �������� ���������.
    +-----------------------+--------------------------------------------------------------+--------------------------------------------------------+
    | �������               | ������ �������                                               | ��������                                               |
    +-----------------------+--------------------------------------------------------------+--------------------------------------------------------+
    | WRITE_SERVICE_COMMAND | COM                                                          |                                                        |
    | SET_PARAMETER_CHANNEL | COM Channel[1] ParameterChannel[1] ��������[4]               | �������� ��������� �������� ������� float �� �������� |
    |                       |                                                              | � ��������                                             |
    | SET_FORM_CHANNEL      | COM Channel[1] FormChannel[1]                                |                                                        |
    +-----------------------+--------------------------------------------------------------+--------------------------------------------------------+
    \endcode
    
    \code
    * ����� � ���������� ������� �������� ���������� ����.
    \endcode

    \section sec13 3. �������� ������ �� SPI ����� ������������
	
	����� ������� ������������ �������� �������� 64 �����.
	
	��������� ������.
	
	\code
					+--------+--------------+----------------------+
					| CRC32  | ����� ������ | ���-�� ���� � ������ |
	+---------------+--------+--------------+----------------------+
    | ������ ������ | 0...3  |    4...7     |       8...11         |
    +---------------+--------+--------------+----------------------+
	\endcode
    
    � ���� <b>CRC32</b> ��������� ����������� ����� ������, ������� � 4 �� 63-�.
    
    <b>����� ������</b> - ���������� ����� ������������� ������. ������ ����� ���� ��������� ��� ����������� � ��������.

    \section sec14 4. ������ ������� ������������ ����� � ����.

    ��� ������ ������� ������������ ����� ����������� 2 ����� ������ 14 ��� �� ������ �����. ������ ���� ������ ����� ����� 8��. ����� ������ 
    ������������� ���������������:\n
    RAM0 - ������� 8 ��� ������ �;\n
    RAM1 - ������� 6 ��� ������ �;\n
    RAM2 - ������� 8 ��� ������ �;\n
    RAM3 - ������� 6 ��� ������ �.\n
    ��� ������ ������ ������ ����� ����� ���������� ��� RG0.0 � "0". ����� ����� ������� ������, ������� ������������ ��� ������ �������, 
    ��������������� � 0. ��������� �������� ���������������� �������� ����������� WR - ���� PD5 - ������������� ��������� 0-1-0. ������������������ 
    ������ ������:\n
    - ���������� ������ D7...D0 �� ������� �����������;\n
    - �������� 1...2���;\n
    - �������� ������ ���� ������ �������� WR;\n
    - ������� ������� ������� WR �������� ������� ���� ����������������;\n
    - �������� 1...2���;\n
    - ����������� ��� �������� �� ��� ���, ���� �� ����� �������� ��� 4 ����� ������;\n
    - �� ��������� ������ ��� RG0.0 ��������������� � ��������� "1".\n
        ������ � ������ ������ ������������ � ������ ���� ��� ���������� ������ ���������� ����������� ����:\n
        0XXXXXXXXXXXXX - ������������� �������� [0...8191];\n
        1XXXXXXXXXXXXX - ������������� �������� [-8191...-0].\n

    \section sec15 5. ���������� ��������������

    MX1 - ���������� ������� ������
    MX2 - ���������� ������� ������

    \code
    +----------------+-----------+------------------------------------------------------------------------------------+
    | ����           | ��������� |                         ��������                                                   |
    | �������������� |           |                                                                                    |
    +----------------+-----------+------------------------------------------------------------------------------------+
    | MX1.0          |    1      | �� ����� ������ 1 ������� ����� (� ��� ����� � ���������������� ������ ���������) |
    | ��1.0          |    0      | �� ����� ������ 1 �������� ��� ��������� �������                                   |
    | MX2.0          |    1      | �� ����� ������ 2 ������� ����� (� ��� ����� � ���������������� ������ ���������) |
    | ��2.0          |    0      | �� ����� ������ 2 �������� ��� ��������� �������                                   |
    +----------------+-----------+------------------------------------------------------------------------------------+
    \endcode

    \section sec16 6. ����������� �������������� ��������

    ��� ���������� �������� ����������� ������������ ���� 3, 4, 5, 6 �������� ���������� RG0 ����.

    \code
    +-------+------+--------------------------------------------------------------------------------------+
    |  RG0  |  MX  |                                                                                      |
    +---+---+------+---------+                                                                            |
    | 3 | 4 | 1[0] | ����� 1 |                          ��������                                          |
    | 5 | 6 | 2[0] | ����� 2 |                                                                            |
    +---+---+------+---------+----------------------------------------------------------------------------+
    | 1 | 1 |  0   | ����������� ���������, ������ �� ������ ��������� ����                               |
    | 1 | 1 |  1   | ������ �� ������ ��������� AD9952 (���������), ����������� ���������                 |
    | 0 | 1 |  1   | ������ �� ������ ��������� AD9952, ����������� �������� OSK ("����")                 |
    | 1 | 0 |  1   | ������ �� ������ ��������� AD9952, ����������� �������������� ��������� ����� ������ |
    +---+---+------+--------------------------------------------------------------------------------------+
    \endcode
*/
