#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */

int process_message(char *arrc_buffer){
    /**
     * Process the message
     */
    int n_command_index = -1;
    if (arrc_buffer[1] == '4'){
        /**
         * gps
         */
        n_command_index = 4;
#ifdef PRINT_DEBUG_UART_MESSAGE
        printf("gps received\n");
#endif
        char arrc_face_direction[4] = {'\0'};
        char arrc_latitude[12] = {'\0'};
        char arrc_longitude[12] = {'\0'};
        int nflag_load_face_direction = 0;
        int nflag_load_latitude = 0;
        int nflag_load_longitude = 0;
        int n_message_index = 2;
        int n_face_direction_index = 0;
        int n_latitude_index = 0;
        int n_longitude_index = 0;
        while (nflag_load_face_direction == 0 || nflag_load_latitude == 0 || nflag_load_longitude == 0){
            if (nflag_load_face_direction == 0){
                if (arrc_buffer[n_message_index] == '|'){
                    arrc_face_direction[n_face_direction_index] = '\0';
                    nflag_load_face_direction = 1;
                    n_message_index++;
                }else{
                    arrc_face_direction[n_face_direction_index] = arrc_buffer[n_message_index];
                    n_face_direction_index++;
                    n_message_index++;
                    nflag_load_face_direction = 0;
                }
            }else if (nflag_load_latitude == 0){
                if (arrc_buffer[n_message_index] == '|'){
                    arrc_latitude[n_latitude_index] = '\0';
                    nflag_load_latitude = 1;
                    n_message_index++;
                }else{
                    arrc_latitude[n_latitude_index] = arrc_buffer[n_message_index];
                    n_latitude_index++;
                    n_message_index++;
                    nflag_load_latitude = 0;
                }
            }else if (nflag_load_longitude == 0){
                if (arrc_buffer[n_message_index] == '$'){
                    arrc_longitude[n_longitude_index] = '\0';
                    nflag_load_longitude = 1;
                    n_message_index++;
                }else{
                    arrc_longitude[n_longitude_index] = arrc_buffer[n_message_index];
                    n_longitude_index++;
                    n_message_index++;
                    nflag_load_longitude = 0;
                }
            }
        }
        printf("%d\n", atoi(arrc_face_direction));
        printf("%f\n", atof(arrc_latitude));
        printf("%f\n", atof(arrc_longitude));
    }
    return 0;
}

int main(){
    int a = 2;
    double b = 012.345;
    double c = -124.944321432;
    char sz_char[31] = {'\0'};
    sprintf(sz_char, "~4%d|%.6f|%.6f$", a, b, c);
    printf("%s\n", sz_char);
    process_message(sz_char);
	return 0;
}
