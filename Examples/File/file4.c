#include <stdio.h>

int main(){

FILE *fp;
struct emp{

	char name[40]; int age; float bs;
};

struct emp e[10];

fp = fopen("EMPLOYEE.DAT","w");

for(int i=0; i<9; i++){
	fprintf(fp,"%s %d %f\n",e->name,e->age,e->bs);
}
fclose(fp);

fp = fopen("EMPLOYEE.DAT","r");
while(fscanf(fp, "%s %d %f", e->name, &e->age, &e->bs) != EOF){
	printf("%s %d %f\n",e->name,e->age,e->bs);}
fclose(fp);

fp = fopen("EMP.DAT","wb");
for(int i=0; i<9; i++){
	fwrite(&e, sizeof(e),1,fp);
}
fclose(fp);

fp = fopen("EMP.DAT","rb");
while(fread(&e,sizeof(e),1,fp) == 1){
	printf("%s %d %f\n",e->name,e->age,e->bs);
}

fclose(fp);

return 0;
}