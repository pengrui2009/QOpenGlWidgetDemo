#include "modelloader.h"

ModelLoader::ModelLoader()
{
    memset(&path, 0x00, sizeof(path));
}

int ModelLoader::Load(char *filepath, char* filename, int loadtype){
    char line[100];
    char mark[5];
    char tmp[50];
    char mtllib[20];
    char Materialname[20];
    int MaterialID = 0;
    float x,y,z;
    float u,v;
    char *pch;
    std::vector<int> faceIndex;
    std::vector<std::vector<int> > face;
    int ix,iy,iz;

    strcpy(path, filepath);
    char fullname[200] = {0};
    sprintf(fullname,"%s/%s", path, filename);
    FILE* fp = fopen(fullname, "r");
    printf(" filepath: %s\n",  fullname);

    //V.push_back(Point(0,0,0));

    if(!fp){
        printf("Couldn't open the file!\n");
        return -1;
        //exit(1);
    }

    while(fgets(line, 100, fp)){
        switch(line[0]){
        case 'v':
            if (line[1] == ' '){
                sscanf(line, "%s %f %f %f", mark, &x, &y, &z);
                V.push_back(Point(x,y,z));
                printf("%s    %f %f %f\n",mark, x, y, z);
                break;
            }
            else if (line[1] == 't'){
                sscanf(line, "%s %f %f", mark, &u, &v);
                Vt.push_back(Texture(u,v));
                printf("%s    %f %f\n",mark,u,v);
                break;
            }
            else if (line[1] == 'n'){
                sscanf(line, "%s %f %f %f", mark, &x, &y, &z);
                Vn.push_back(Normal(x,y,z));
                printf("%s    %f %f %f\n",mark, x, y, z);
                break;
            }
        case 'f':
            pch = strtok (line," ");
            pch = strtok (NULL, " ");
            if (loadtype == 1){
                while (pch != NULL)
                {
                    sscanf(pch, "%d/%d/%d %s", &ix, &iy, &iz, &tmp);
                    faceIndex.push_back(ix);
                    faceIndex.push_back(iy);
                    faceIndex.push_back(iz);
                    face.push_back(faceIndex);
                    faceIndex.clear();
                    pch = strtok (NULL, " ");
                    printf("%d/%d/%d\n", ix, iy, iz);
                }
            }
            else{
                while (pch != NULL)
                {
                    sscanf(pch, "%d/%d %s", &ix, &iy, &tmp);
                    faceIndex.push_back(ix);
                    faceIndex.push_back(iy);
                    face.push_back(faceIndex);
                    faceIndex.clear();
                    pch = strtok (NULL, " ");
                }
            }
            F.push_back(Face(face[0],face[1],face[2],face[3],MaterialID));
            face.clear();
            printf("successfully load 'f'! \n");
            break;

        case 'm':
            pch = strtok (line," ");
            pch = strtok (NULL, " ");
            while (pch != NULL)
            {
                sscanf(pch, "%s", &mtllib);
                pch = strtok (NULL, " ");
            }
            if(LoadMTL(mtllib)==0){
                printf("successfully load 'mtllib'! \n");
            }
            else{
                printf("fail");
                return -1;
            }
            break;

        case 'u':
            pch = strtok (line," ");
            pch = strtok (NULL, " ");
            while (pch != NULL)
            {
                sscanf(pch, "%s", &Materialname);
                for (auto MatIndex = M.begin(); MatIndex != M.end(); ++MatIndex){
                    if (strcmp(MatIndex->name, Materialname)==0){
                        MaterialID = MatIndex->MaterialID;
                    }
                }

                pch = strtok (NULL, " ");
            }
            printf("successfully load 'u'! \n");
            break;

        default:
            break;
        }
    }
    fclose(fp);
    return 0;
}




int ModelLoader::LoadMTL(char* mtllib){
    char *pch;
    char line[100];
    char Materialname[20],mapkd[20],mapks[20],mapbump[20],mapka[20];
    char MaterialID = 0;
    int il;
    float ns, ka1,ka2,ka3, ks1,ks2,ks3, kd1,kd2,kd3;
    std::vector<float> ka, ks, kd;
    Material* tmp = NULL;

    char fullname[100];
    sprintf(fullname,"%s/%s", path, mtllib);
    FILE* fm = fopen(fullname, "r");
    printf("filepath: %s", fullname);

    if(!fm){
        printf("Couldn't open the mtl file!\n");
        return -1;
        //exit(1);
    }

    while(fgets(line, 100, fm)){
        switch(line[0]){
        case 'n':
            if (MaterialID>0){
                M.push_back(*tmp);
                free(tmp);
                tmp = NULL;
            }
            MaterialID++;
            tmp = new Material(MaterialID);
            pch = strtok (line," ");
            pch = strtok (NULL, " ");
            while (pch != NULL)
            {
                sscanf(pch, "%s", &Materialname);
                strcpy(tmp->name, Materialname);
                pch = strtok (NULL, " ");
            }
            printf("1\n%d %s\n", MaterialID, Materialname);
            break;

        case 'N':
            if (line[1]=='s'){
                pch = strtok (line," ");
                pch = strtok (NULL, " ");
                while (pch != NULL)
                {
                    sscanf(pch, "%f", &ns);
                    tmp->Ns = ns;
                    pch = strtok (NULL, " ");
                }
                printf("2\n ns: %f\n",ns);
            }
            break;

        case 'K':
            if (line[1]=='a'){
                pch = strtok (line," ");
                pch = strtok (NULL, " ");

                while (pch != NULL)
                {
                    char leftstr[20];
                    sscanf(pch, "%f %s", &ka1, &leftstr);
                    printf("ka: %f\n",ka1);
                    ka.push_back(ka1);
                    pch = strtok (NULL, " ");
                }
                tmp->Ka = ka;
                ka.clear();
            }
            else if (line[1]=='d'){
                pch = strtok (line," ");
                pch = strtok (NULL, " ");
                while (pch != NULL)
                {
                    char leftstr[20];
                    sscanf(pch, "%f %s", &kd1, &leftstr);
                    printf("kd: %f\n",kd1);
                    kd.push_back(kd1);
                    pch = strtok (NULL, " ");
                }
                tmp->Kd = kd;
                kd.clear();
            }
            else if (line[1]=='s'){
                pch = strtok (line," ");
                pch = strtok (NULL, " ");
                while (pch != NULL)
                {
                    char leftstr[20];
                    sscanf(pch, "%f %s", &ks1, &leftstr);
                    printf("ks: %f\n",ks1);
                    ks.push_back(ks1);
                    pch = strtok (NULL, " ");
                }
                tmp->Ks = ks;
                ks.clear();
            }
            break;

        case 'i':
            pch = strtok (line," ");
            pch = strtok (NULL, " ");
            while (pch != NULL)
            {
                sscanf(pch, "%d", &il);
                tmp->illum = il;
                pch = strtok (NULL, " ");
            }
            printf("4\n illum: %d\n", il);
            break;

        case 'm':
            if(line[5]=='d'){
                //printf("%c", line[5]);
                pch = strtok (line," ");
                pch = strtok (NULL, " ");
                while (pch != NULL)
                {
                    sscanf(pch, "%s", &mapkd);
                    tmp->map_Kd = LoadBMP(mapkd);
                    pch = strtok (NULL, " ");
                }
            }
            else if(line[5]=='u'){
                //printf("%c", line[5]);
                pch = strtok (line," ");
                pch = strtok (NULL, " ");
                while (pch != NULL)
                {
                    sscanf(pch, "%s", &mapbump);
                    tmp->map_Bump = LoadBMP(mapbump);

                    pch = strtok (NULL, " ");
                }
            }
            else if(line[5]=='s'){
                //printf("%c", line[5]);
                pch = strtok (line," ");
                pch = strtok (NULL, " ");
                while (pch != NULL)
                {
                    sscanf(pch, "%s", &mapks);
                    tmp->map_Ks = LoadBMP(mapks);

                    pch = strtok (NULL, " ");
                }
            }
            else if(line[5]=='a'){
                //printf("%c", line[5]);
                pch = strtok (line," ");
                pch = strtok (NULL, " ");
                while (pch != NULL)
                {
                    sscanf(pch, "%s", &mapka);
                    tmp->map_Ka = LoadBMP(mapka);

                    pch = strtok (NULL, " ");
                }
            }
            printf("5\n");
            break;

        default:
            printf("default\n");
            break;

        }
    }
    if (tmp != NULL){
        M.push_back(*tmp);
    }

    fclose(fm);
    return 0;
}


GLuint ModelLoader::LoadBMP(char *filename)
{
    GLuint tex_id;
    GLint alignment;

    QImage tex, buf;

    char fullname[100];
    sprintf(fullname,"%s",filename);
    buf.load(fullname);
    printf("%s\n", fullname);

    tex = QGLWidget::convertToGLFormat(buf);

    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//GL_CLAMP
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//GL_REPEAT
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glGetIntegerv (GL_UNPACK_ALIGNMENT, &alignment);
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
        GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
    glPixelStorei (GL_UNPACK_ALIGNMENT, alignment);

    printf("%d\n", tex_id);
    return tex_id;
}




void ModelLoader::Draw(int f_type){
    glPushMatrix();
    glScalef(10.0f,10.0f,10.0f);

    GLfloat color1[11] = {0.299f,0.022f,0.6f,0.0f,0.972f,0.545f,0.45f,0.23f,0.15f,0.870f,0.972f};
    GLfloat color2[11] = {0.507f,0.075f,0.6f,0.0f,0.972f,0.474f,0.45f,0.22f,0.15f,0.720f,0.972f};
    GLfloat color3[11] = {0.700f,0.088f,0.6f,0.0f,0.999f,0.368f,0.45f,0.20f,0.15f,0.520f,0.999f};

    for (auto faceIndex = F.begin(); faceIndex != F.end(); ++faceIndex){
        int mid = faceIndex->MaterialID;

        glEnable(GL_DEPTH_TEST);
        //glDepthMask(GL_FALSE);
        if (mid != 0){
            float glKa[4] = {M[mid-1].Ka[0],M[mid-1].Ka[1],M[mid-1].Ka[2],M[mid-1].Tr};
            float glKs[4] = {M[mid-1].Ks[0],M[mid-1].Ks[1],M[mid-1].Ks[2],M[mid-1].Tr};
            float glKd[4] = {M[mid-1].Kd[0],M[mid-1].Kd[1],M[mid-1].Kd[2],M[mid-1].Tr};
            float glNs[1] = {M[mid-1].Ns};
            //printf("%f %f %f\n", glKa[0],glKa[1],glKa[2]);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glKa);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glKs);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glKd);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, glNs);

            glColor3f(color1[mid-1],color2[mid-1],color3[mid-1]);
            if(M[mid-1].map_Kd!=0){
                printf("bind\n");
                switch (f_type) {
                case 1:
                    glBindTexture(GL_TEXTURE_2D, M[mid-1].map_Kd);
                    break;
                case 2:
                    glBindTexture(GL_TEXTURE_2D, M[mid-1].map_Ka);
                    break;
                case 3:
                    glBindTexture(GL_TEXTURE_2D, M[mid-1].map_Ks);
                    break;
                default:
                    break;
                }
                glColor3f(1.0f,1.0f,1.0f);
            }

            //glBindTexture(GL_TEXTURE_2D, M[mid-1].map_Kd);
            //glColor3f(1.0f,1.0f,1.0f);
            //printf("%d\n", M[mid-1].map_Kd);
            //glBindTexture(GL_TEXTURE_2D, M[mid-1].map_Ks);
            //printf("%d\n", M[mid-1].map_Ks);
            //glBindTexture(GL_TEXTURE_2D, M[mid-1].map_Bump);
            //printf("%d\n", M[mid-1].map_Bump);
        }
        //glColor3f(0.1f,0.1f,0.1f);

        glBegin(GL_POLYGON);
        //printf("%d\n", faceIndex->f.size());

        //1
        glNormal3f(Vn[faceIndex->f[0][2]-1].vn[0],Vn[faceIndex->f[0][2]-1].vn[1],Vn[faceIndex->f[0][2]-1].vn[2]);
        glTexCoord2f(Vt[faceIndex->f[0][1]-1].vt[0],Vt[faceIndex->f[0][1]-1].vt[1]);
        glVertex3f(V[faceIndex->f[0][0]-1].v[0],V[faceIndex->f[0][0]-1].v[1],V[faceIndex->f[0][0]-1].v[2]);

        //2
        glNormal3f(Vn[faceIndex->f[1][2]-1].vn[0],Vn[faceIndex->f[1][2]-1].vn[1],Vn[faceIndex->f[1][2]-1].vn[2]);
        glTexCoord2f(Vt[faceIndex->f[1][1]-1].vt[0],Vt[faceIndex->f[1][1]-1].vt[1]);
        glVertex3f(V[faceIndex->f[1][0]-1].v[0],V[faceIndex->f[1][0]-1].v[1],V[faceIndex->f[1][0]-1].v[2]);

        //3
        glNormal3f(Vn[faceIndex->f[2][2]-1].vn[0],Vn[faceIndex->f[2][2]-1].vn[1],Vn[faceIndex->f[2][2]-1].vn[2]);
        glTexCoord2f(Vt[faceIndex->f[2][1]-1].vt[0],Vt[faceIndex->f[2][1]-1].vt[1]);
        glVertex3f(V[faceIndex->f[2][0]-1].v[0],V[faceIndex->f[2][0]-1].v[1],V[faceIndex->f[2][0]-1].v[2]);

        //4
        glNormal3f(Vn[faceIndex->f[3][2]-1].vn[0],Vn[faceIndex->f[3][2]-1].vn[1],Vn[faceIndex->f[3][2]-1].vn[2]);
        glTexCoord2f(Vt[faceIndex->f[3][1]-1].vt[0],Vt[faceIndex->f[3][1]-1].vt[1]);
        glVertex3f(V[faceIndex->f[3][0]-1].v[0],V[faceIndex->f[3][0]-1].v[1],V[faceIndex->f[3][0]-1].v[2]);

        glEnd();

        glDisable(GL_DEPTH_TEST);
    }
    glFlush();
    glPopMatrix();
}
