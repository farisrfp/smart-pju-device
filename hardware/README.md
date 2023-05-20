# **Hardware Documentation**

## **BOM**

The BoM (Bill of Material) for this project can be seen in the table below.

### **Power Board**

| Designator | Quantity | Description |
| :--------: | :------: | :---------: |
|   AC-DC    |    1     |  AC to DC   |
|     V1     |    1     |  Varistor   |
|     F1     |    1     | Fuse Holder |
|     F2     |    1     |  Fuse PTC   |
|    DIM     |    1     |    PC817    |
|    IC1     |    1     | ACS712-05B  |
|    IC2     |    1     |   LM358N    |
|     T1     |    1     |  ZMPT101B   |
|    SSR     |    1     |  G3MB-202P  |

**Passive Components**

| Designator | Quantity |  Package  |  Value  |
| :--------: | :------: | :-------: | :-----: |
|     Q1     |    1     |  SOT-23   | SS8550  |
|     D1     |    1     |  SOD-123  | 1N4448W |
|     D2     |    1     | DO-214AC  |   M4    |
|    LED     |    1     |   0805    |  Blue   |
|     R7     |    1     |   3296    |  100K   |
|     R1     |    1     |   0805    |   100   |
|   R2 - 3   |    2     |   0805    |   330   |
|     R4     |    1     |   0805    |   2K    |
|   R5 - 6   |    2     |   0805    |  4,7K   |
|   R8 - 9   |    2     |   0805    |  100K   |
|  R10 - 17  |    8     |   0805    |   10K   |
|  R19 - 20  |    2     | Axial-7.2 |  330K   |
|   C1 - 2   |    2     |   0805    |   1u    |
|   C3 - 4   |    2     |   0805    |  100n   |
|   C5 - 8   |    4     |   0805    |   1n    |

<br>

### **Controller Board**

| Designator | Quantity |       Description        |
| :--------: | :------: | :----------------------: |
|   ESP32    |    1     |       ESP32-WROOM        |
|  REG-3V3   |    1     |       AMS1117-3V3        |
|   RA-01H   |    1     |    AI-Thinker RA-01H     |
|   DS3231   |    1     |    RTC DS3231 Module     |
|   NEO 1    |    2     |         WS2812B          |
|   S1 - 2   |    2     |       Micro Button       |
|     S3     |    1     |        DIP Switch        |
|    PH1     |    1     | Light Dependent Resistor |

**Passive Components**

| Designator | Quantity | Package | Value |
| :--------: | :------: | :-----: | :---: |
|    LED     |    1     |  0805   |  Red  |
|   R1 - 2   |    2     |  0805   |  1K   |
|     R3     |    1     |  0805   |  2K   |
|   R4 - 6   |    3     |  0805   |  10K  |
|     C1     |    1     |  0805   | 100n  |
|     C2     |    1     |  0805   |  1u   |
|   C3 - 4   |    2     |  0805   |  10u  |
|     C6     |    1     |  2412   | 100u  |
