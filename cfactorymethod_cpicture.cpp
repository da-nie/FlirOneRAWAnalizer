#include "cfactorymethod_cpicture.h"

//----------------------------------------------------------------------------------------------------
//������� ������ �����������
//----------------------------------------------------------------------------------------------------
CIImage* CFactoryMethod_CPicture::Create_CIImage(void)
{
 return(new(CPicture));
}