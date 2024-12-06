#ifndef __LCD_Basic_application_H__
#define __LCD_Basic_application_H__


void DrawPoint(uint16_t x,uint16_t y,uint16_t color);
void DrawLine(uint16_t xStar,uint16_t yStar,uint16_t xEnd,uint16_t yEnd,uint16_t color);
void DrawHollowRectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
void DrawSolidRectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
void DrawCircular_SymmetryPoint(uint16_t x_centre,uint16_t y_centre,uint16_t x_edge,uint16_t y_edge,uint16_t color);
void DrawHollowCircular(uint16_t x_centre,uint16_t y_centre,uint16_t Radius,uint16_t color);
void DrawSolidCircular(uint16_t x_centre,uint16_t y_centre,uint16_t Radius,uint16_t color);
void ShowCP(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, uint8_t *str);
void ShowYW24(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, char *str);
void ShowChar(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, char str,uint8_t size,uint8_t mode);  
void ShowString(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc,char *s,uint8_t size,uint8_t mode);		 
void DrawFont16(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc,char *s,uint8_t mode);
void DrawFont24(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc,char *s,uint8_t mode);
void DrawFont32(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc,char *s,uint8_t mode);   		   
void Show_Str(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, char *str,uint8_t size,uint8_t mode);


#endif

