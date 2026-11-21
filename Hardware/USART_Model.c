#include "USART_Model.h"   

//定义环形缓冲的数组
char DMA_Buffer[BUFFER_SIZE];
//定义下一次搬运的数组的开始位置的下标
uint16_t lastPosition=0;


//初始化DMA 的函数，使用DMA 来转运USART 的数据寄存器中的数据
void USART2_DMA_Init(void){
	//开时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	//配置DMA
	DMA_InitTypeDef DMA_InitStruct;
	
	//============外设================
	DMA_InitStruct.DMA_PeripheralBaseAddr=(uint32_t)&USART2->DR;	//USART 数据寄存器的地址
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;	//数据项的大小，字节
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;	//外设的地址不是自动递增
	
	//==========存储器================
	DMA_InitStruct.DMA_MemoryBaseAddr=(uint32_t)DMA_Buffer;
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;	//数据项大小，字节(8bit) 字（32bit）半字（16bit）
	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;	//数组中元素的地址是连续
	
	//=========其他属性===============
	DMA_InitStruct.DMA_BufferSize=BUFFER_SIZE;//数据长度
	DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralSRC;	//方向，外设是源，USART2->DR ===>数组
	DMA_InitStruct.DMA_M2M=DMA_M2M_Disable;		//存储器到存储器，DMA_M2M_Disable 表示不是存储器到存储器	
	DMA_InitStruct.DMA_Mode=DMA_Mode_Circular;	//循环模式，每次计完，不会恢复成原始值，再计数器当前值的基础上继续向下计数
	DMA_InitStruct.DMA_Priority=DMA_Priority_Medium;//优先级
	
	//根据手册10.3.7,得知 使用DMA1 的通道5
	DMA_Init(DMA1_Channel6,&DMA_InitStruct);
	
	//使能DMA
	DMA_Cmd(DMA1_Channel6,ENABLE);



}





//初始化USART
void USART_Model_Init(void){
	//开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;	//浮空或者上拉
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//配置USART 起始位 1 + 数据位 8 + 校验位无+停止位1位
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate=9600;	//波特率
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;	//无流量控制
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity=USART_Parity_No;	//无校验位
	USART_InitStruct.USART_StopBits=USART_StopBits_1;	//停止位是1
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;	//数据长度
	USART_Init(USART2,&USART_InitStruct);
	
	//初始化DMA 
	USART2_DMA_Init();
	//USART 请求DMA转运，USART_DMAReq_RX 接收数据
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
	
	//配置IDLE 中断，IDLE 空闲中断
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
	
	//配置NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);
	
	//使能USART
	USART_Cmd(USART2,ENABLE);
}

//发送字节,timeOut表示超时时间
void Send_Byte(uint8_t data,uint32_t timeOut){
	USART_SendData(USART2,data);
	uint32_t time=0;
	
	//TXE 如果为0,表示数据尚未发送到移位寄存器，表示数据没有发出去
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){
		if(time<timeOut){
			//没有超时，正常继续发送数据
			time+=5;
			delay_by_ms(5);
		}
		else {
		//超时
		break;
		}
	}
	
}


//发送数组
void Send_Array(uint8_t *arr,uint16_t length){
	for(uint16_t i=0;i<length;i++){
		Send_Byte(arr[i],100);
	}
}

//发送字符串
void Send_String(char *str){
	while(*str!='\0'){
		Send_Byte(*str++,100);
	}

}



//覆盖fputc 函数
int fputc(int ch ,FILE *fp){
	Send_Byte(ch,100);
	return ch;
}


//定义字符数组
char message[200]={0};
//定义状态表示
uint8_t flag=0;

void handleDate(void){
	//每次接收都清零，防止残留旧数据
	memset(message, 0, sizeof(message));

	//获取DMA当前写位置，必须做%环形回绕！
	//如果DMA计数器刚好到0（重装前瞬间），(200-0)%200=0，确保不会出现200这种越界值
	uint16_t currWritePos = (BUFFER_SIZE - DMA_GetCurrDataCounter(DMA1_Channel6)) % BUFFER_SIZE;

	//如果没有新数据，直接返回
	if(currWritePos == lastPosition){
		return;
	}

	//定义message数组中使用的下标
	uint16_t index = 0;
	//循环搬运数据，直到到达当前DMA写位置
	while(lastPosition != currWritePos){
		char ch = DMA_Buffer[lastPosition++];
		if(lastPosition >= BUFFER_SIZE){
			lastPosition = 0;
		}

		//过滤 \r \n 等控制字符，防止OLED_ShowChar越界访问字库数组
		if(ch == '\r' || ch == '\n'){
			continue;
		}
		//防溢出，message留1字节给'\0'
		if(index < sizeof(message) - 1){
			message[index++] = ch;
		}
	}
	//在message最后位置放\0
	message[index] = '\0';

	//通知主循环，数据已就绪
	flag = 1;
	//根据串口收到的数据给机械中值，KP KD 重新赋值
	if(strcmp(message,"1")==0){
		MachineBalance++;
	}
	else if(strcmp(message,"2")==0){
		MachineBalance--;
	}
	else if(strcmp(message,"3")==0){
		BalanceKP++;
	}
	else if(strcmp(message,"4")==0){
		BalanceKP--;
	}
	else if(strcmp(message,"5")==0){
		BalanceKD=BalanceKD+0.1;
	}
	else if(strcmp(message,"6")==0){
		BalanceKD=BalanceKD-0.1;
	}
	else if(strcmp(message,"7")==0){
		MachineBalance=MachineBalance+0.1;
	}
	else if(strcmp(message,"8")==0){
		MachineBalance=MachineBalance-0.1;
	}
	else if(strcmp(message,"9")==0){
		BalanceKP=BalanceKP+10;
	}
		else if(strcmp(message,"10")==0){
		BalanceKP=BalanceKP-10;
	}
	
}

void USART2_IRQHandler(void){
	if(USART_GetITStatus(USART2,USART_IT_IDLE)==SET){
		USART_ClearITPendingBit(USART2,USART_IT_IDLE);	
		uint32_t temp;
		temp=(uint32_t)USART2->SR;
		temp=(uint32_t)USART2->DR;
		(void)temp;

		//定义一个函数用来将DMA_Buffer 环形缓冲数组中的lastPosition 位置中间的元素搬到message 中
		handleDate();
		
		//清除空闲中断标志位
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	
	}
}















/*
void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1,USART_IT_IDLE)==SET){
		USART_ClearITPendingBit(USART1,USART_IT_IDLE);	
		uint32_t temp;
		temp=(uint32_t)USART1->SR;
		temp=(uint32_t)USART1->DR;
		(void)temp;
		//=================给数组最后位置设置结束符=========
		//获取DMA 计数器的递减的值，BUFFERSIZE-cnt 的值，就是字符串结束的位置
		//DMA_GetCurrDataCounter----获取DMA 计数器的值
		uint16_t dmaCurrCnt =DMA_GetCurrDataCounter(DMA1_Channel5);
		//message 中BUFFERSIZE-dmaCurrCnt的位置设置为\0
		message[BUFFER_SIZE-dmaCurrCnt]='\0';
		flag=1;		//通知主循环数据已就绪
		//恢复DMA 的计数器的值为原始值，DMA_SetCurrDataCounter----设置当前的计数器的值
		DMA_SetCurrDataCounter(DMA1_Channel5,BUFFER_SIZE);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	
	}
}
	
*/	
/*
//指定中断函数
void USART1_IRQHandler(void){
	//定义数组的小标，局部变量是放栈中，静态局部变量是放在数据段中，函数执行完不会自动释放
	static uint8_t index=0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET){
		uint8_t data =USART_ReceiveData(USART1);
		message[index++]=data;
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
	else if(USART_GetITStatus(USART1,USART_IT_IDLE)==SET){
		USART_ClearITPendingBit(USART1,USART_IT_IDLE);	
		//在数组最后位置存放一个结束符\0
		message[index]='\0';
		//index 恢复成0
		index=0;
		//flag 设置成1
		flag=1;

		uint32_t temp;
		temp=(uint32_t)USART1->SR;
		temp=(uint32_t)USART1->DR;
		
		(void)temp;
		
		
		
	}

}


*/

































