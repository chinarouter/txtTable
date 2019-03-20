/*
 * Description:  解析文本格式表格数据
 * Author     :  maohw (), maohongwei@gmail.com
 * Company    :  gsf
 */

#ifndef __txt_table_h__
#define __txt_table_h__

#if 0 // table.txt
# '#'开始行为注释行;
# '空格'为分割符,第一行定义表头,表头格式[ i整型 f浮点型 s字符串型 h十六进制]
#
  iPOS  f公称倍率   i水平视场角  i垂直视场角          s备注         h测试
	0   1.0 	    263598 	  	 153708         123456        0x1846
    1   1.1	  	    263576	  	 153693         123456        0x1A7B
	3   1.2  	    263534 	  	 153665         123456        0x1CB0
	4   1.3	 	    263513	  	 153651 	    123456        0x1EE5
	6   1.4  	    263471 	  	 153623 	    123456        211A
	7   1.5  	    263449   	 153609         abcdefg       234F
	9   1.6  	    263407 	  	 153581         abcdefg       2584
	10  1.7  	    263386 	 	 153567         abcdefg       27B9
	12  1.8  	    263344 	  	 153538         abcdefg       29EE
#endif


#define __TEST_MAIN__   // 定义内部测试用main函数;

/*
 * 描述: 装载 table.txt
 * name: 文件名
 * cb  : 字段回调
 */
int txt_table(char *name, int(*cb)(int row, int col, char fmt, void *v, void *u), void *u);


#endif //__txt_table_h__