# CanStepDriver

### Как сделать сборку библиотеки .a из проекта CubeMX
1. В проекте поставить галочку "Do not generate the main()", сгенерировать проект
2. Переименовать **main.c** в **ll_main.c**
3. Makefile:
- Добавить к переменной **C_SOURCES** файлы **LowLevel.c** и **Core/Src/ll_main.c**, и удалить из неё файл **main.c**
```shell
C_SOURCES =  \
LowLevel.c \
Core/Src/ll_main.c \
```

- Добавить к переменной **C_INCLUDES** директорию /LowLevel **-I../ \**
```shell
C_INCLUDES =  \
-I../ \
-ICore/Inc \
```

- Добавить к цели clean удаление библиотеки **-rm -f lib$(TARGET).a**
```shell
clean:
	-rm -fR $(BUILD_DIR)
	-rm -f lib$(TARGET).a
```
- Добавить в секцию "build the application" сборку библиотеки **lib:**

```shell
#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

lib: $(OBJECTS) Makefile
	arm-none-eabi-ar -rc lib$(TARGET).a $(OBJECTS)
```

4. Создать файл **LowLevel.c** и наполнить функциями
5. Добавить в **main.h**
- Прототип ***SystemClock_Config*
```cpp
/* USER CODE BEGIN EFP */
void SystemClock_Config(void);
/* USER CODE END EFP */
```

- Нужные дефайны
```cpp
/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
#define RS485_DIR_Pin GPIO_PIN_0
#define RS485_DIR_GPIO_Port GPIOA
#define LED_RED_Pin GPIO_PIN_3
#define LED_RED_GPIO_Port GPIOA
#define LED_BLUE_Pin GPIO_PIN_6
#define LED_BLUE_GPIO_Port GPIOA
#define LED_GREEN_Pin GPIO_PIN_11
#define LED_GREEN_GPIO_Port GPIOF
#define CAN1_RS_Pin GPIO_PIN_2
#define CAN1_RS_GPIO_Port GPIOD
/* USER CODE END Private defines */
```

6. При использовании FreeRTOS удалить из **stm32f1xx_it.c** реализации этих обработчиков
```shell
void SVC_Handler(void)
void PendSV_Handler(void)
void SysTick_Handler(void)
```

7. Если есть ошибка RWX permissions, то помогает такое
you can update "(READONLY)" into linker file with:
```shell
.fini_array (READONLY):
.init_array (READONLY):
.preinit_array   (READONLY)  :
.ARM (READONLY):
.ARM.extab  (READONLY) :
```

8. Также почему-то syscalls из директории LowLevel не видятся линкером, пришлось копировать их в App в syscalls.cpp

9. С линковкой прерываний может быть проблема, например, файл **stm32f4xx_it.c** не хотел включаться в сборку, пока не вызовешь прерывание из какого-то .cpp файла (вызов внутри LowLevel не помогал). Помогло добавление функции **HalIntprt_Link_Helper** и вызов её из **LowLevel_Init**:

```shell
/* USER CODE BEGIN 1 */
int HalIntprt_Link_Helper(void)
{
  static volatile int dummyFlag = 1;
  return dummyFlag;
}
/* USER CODE END 1 */
```

9. После перегенерирования проекта в CubeMX нужно заново переименовать файл **main.c** в **ll_main.c** и удалить **main.c** из переменной **C_SOURCES** в **Makefile**. Также нужно заново сделать 6 и 7, т.к. изменения сбросятся
