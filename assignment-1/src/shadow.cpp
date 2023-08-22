
void shadowMatrix( const float plane[], const float light_pos[], float* shadow_mat) {
       float dot;

        //Finds dot, which is the product of the light vetcor and the plane's normal
        dot = plane[0] * light_pos[0] +
        plane[1] * light_pos[1] +
        plane[2] * light_pos[2] +
        plane[3] * light_pos[3];

        shadow_mat[0]  = dot - light_pos[0] * plane[0];
        shadow_mat[4]  = -light_pos[0] * plane[1];
        shadow_mat[8]  = -light_pos[0] * plane[2];
        shadow_mat[12] = -light_pos[0] * plane[3];

        shadow_mat[1]  = -light_pos[1] * plane[0];
        shadow_mat[5]  = dot - light_pos[1] * plane[1];
        shadow_mat[9]  = -light_pos[1] * plane[2];
        shadow_mat[13] = -light_pos[1] * plane[3];

        shadow_mat[2]  = -light_pos[2] * plane[0];
        shadow_mat[6]  = -light_pos[2] * plane[1];
        shadow_mat[10] = dot - light_pos[2] * plane[2];
        shadow_mat[14] = -light_pos[2] * plane[3];

        shadow_mat[3]  = -light_pos[3] * plane[0];
        shadow_mat[7]  = -light_pos[3] * plane[1];
        shadow_mat[11] = -light_pos[3] * plane[2];
        shadow_mat[15] = dot - light_pos[3] * plane[3];
}