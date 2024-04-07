#include "math.h"

/*******************************************************************************
* 函 数 名         : Sqrt
* 函数功能		     : 快速算根值
* 输    入         : number 根号里边的数
* 输    出         : 最大的绝对值
*******************************************************************************/
float sqrt(float number)
{
	long i;
	float x, y;
	const float f = 1.5F;
	x = number * 0.5F;
	y = number;
	i = * ( long * ) &y;
	i = 0x5f3759df - ( i >> 1 );

	y = * ( float * ) &i;
	y = y * ( f - ( x * y * y ) );
	y = y * ( f - ( x * y * y ) );
	return number * y;
}
/*******************************************************************************
* 函 数 名         : Angle_Translate
* 函数功能		     : 将角度转变为-180~180之间
* 输    入         : 
* 输    出         : 
*******************************************************************************/
float translateAngle(float angle)
{
	while (angle > ANGLE_MAX)					//判断目标角度是否在允许角度范围
		angle -= 360.0f;
	while (angle < ANGLE_MIN)
		angle += 360.0f;
	return angle;
}
/*******************************************************************************
* 函 数 名         : Sin
* 函数功能		     : 
* 输    入         : 
* 输    出         : 
*******************************************************************************/
float sin(float angle)
{
	float sine, rad;
	angle = translateAngle(angle);
	rad = angle * PI / 180.0f;
	
	sine = (rad < 0) ? rad * (1.27323954f + 0.405284735f * rad) : rad * (1.27323954f - 0.405284735f * rad);
	sine = (sine < 0) ? sine * (-0.225f * (sine + 1) + 1) : sine * (0.225f * (sine - 1) + 1);
	return sine;
}
/*******************************************************************************
* 函 数 名         : Cos
* 函数功能		     : 
* 输    入         : 
* 输    出         : 
*******************************************************************************/
float cos(float angle)
{
	return sin(angle + 90.0f);
}
/*******************************************************************************
* 函 数 名         : Tan
* 函数功能		     : 
* 输    入         : 
* 输    出         : 
*******************************************************************************/
float tan(float angle)
{
	return sin(angle) / cos(angle);
}
/*******************************************************************************
* 函 数 名         : ATan1
* 函数功能		     : 反正切 （+- 0.09°）
* 输    入         : 
* 输    出         : 
*******************************************************************************/
float arctan1(float tan)
{ 
	float angle = (ABS(tan) > 1.0f)	? 																																								\
								90.0f - ABS(1.0f / tan) * (45.0f - (ABS(1.0f / tan) - 1.0f) * (14.0f + 3.83f * ABS(1.0f / tan)))	: \
								ABS(tan) * (45.0f - (ABS(tan) - 1.0f) * (14.0f + 3.83f * ABS(tan)));
	return (tan > 0) ? angle : -angle;
}
/*******************************************************************************
* 函 数 名         : ATan2
* 函数功能		     : 反正切 （+- 0.09°）
* 输    入         : 
* 输    出         : 
*******************************************************************************/
float arctan2(float x, float y)
{
	float tan, angle;
	
	if (x == 0 && y == 0)		//不存在
		return 0;
	
	if (x == 0)							//y轴上
	{
		if (y > 0)
			return 90;
		else
			return -90;
	}
	
	if (y == 0)							//x轴上
	{
		if (x > 0)
			return 0;
		else 
			return -180.0f;
	}
	
	tan = y / x;
	angle = arctan1(tan);
	if (x < 0 && angle > 0)
		angle -= 180.0f;
	else if (x < 0 && angle < 0)
		angle += 180.0f;
	return angle;
}
/*******************************************************************************
* 函 数 名         : arcsin1
* 函数功能		     : 反正切 （+- 0.09°）
* 输    入         : 
* 输    出         : 
*******************************************************************************/
float arcsin(float i)
{
	return arctan1(i / sqrt(1 - i * i));
}
/*******************************************************************************
* 函 数 名         : Exp
* 函数功能		     : e指数
* 输    入         : 
* 输    出         : 
*******************************************************************************/
float exp(float x)	//计算e^x,实现系统的exp()功能?
{
	if (x == 0) return 1;
	if (x<0) return 1 / exp(-x);

	double y = x, ex_p1 = 0, ex_p2 = 0, ex_p3 = 0, ex_px = 0, ex_tmp = 1, dex_px = 1, tmp;
	int l;
	for (l = 1, tmp = 1; ((ex_px - ex_tmp)>1e-10 || (ex_px - ex_tmp)<-1e-10) && dex_px>1e-10; l++)
	{
		ex_tmp = ex_px;
		tmp *= y;
		tmp = tmp / l;
		ex_p1 += tmp;
		ex_p2 = ex_p1 + tmp*y / (l + 1);
		ex_p3 = ex_p2 + tmp*y*y / (l + 1) / (l + 2);
		dex_px = ex_p3 - ex_p2;
		ex_px = ex_p3 - dex_px*dex_px / (ex_p3 - 2 * ex_p2 + ex_p1);
	}
	return ex_px + 1;
}
