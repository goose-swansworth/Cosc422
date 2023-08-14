#include "model_loader.h"
#include "assimp_extras.h"


bool loadModel(const char* fileName, Model* model) {
	aiVector3D scene_min, scene_max, scene_diag;
	model->scene = aiImportFile(fileName, aiProcessPreset_TargetRealtime_MaxQuality);
	if (model->scene == NULL)  {
		cout << "Failed to load model\n";
		exit(1);
	}
	//printSceneInfo(scene);
	//printMeshInfo(scene);
	//printTreeInfo(scene->mRootNode);
	//printBoneInfo(scene);
	//printAnimInfo(scene);  //WARNING:  This may generate a lengthy output if the model has animation data
	get_bounding_box(model->scene, &scene_min, &scene_max);
	model->scene_center = (scene_min + scene_max) * 0.5f;
	scene_diag = scene_max - model->scene_center;  //diagonal vector
	model->scene_scale = 1.0 / scene_diag.Length();
	return true;
}

void loadGLTextures(Model* model)
{
	ILuint imageId;

	if (model->scene == NULL) exit(1);
	if (model->scene->HasTextures())
	{
		std::cout << "Support for meshes with embedded textures is not implemented." << endl;
		exit(1);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glEnable(GL_TEXTURE_2D);
	ilInit();   //DevIL initialization

	model->textureIds = new GLuint[model->scene->mNumMaterials];
	glGenTextures(model->scene->mNumMaterials, model->textureIds);

	for (int m = 0; m < model->scene->mNumMaterials; ++m)
	{
		aiString path;  // filename

		if (model->scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
		{
			ilGenImages(1, &imageId);
			ilBindImage(imageId);
			ilEnable(IL_ORIGIN_SET);
			ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
			if (ilLoadImage((ILstring)path.data))   //if success
			{
				/* Convert image to RGBA */
				ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
				glBindTexture(GL_TEXTURE_2D, model->textureIds[m]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH),
					ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				cout << "Texture:" << path.data << " successfully loaded." << endl;
			}
			else
			{
				cout << "Couldn't load Image: " << path.data << endl;
			}
		}
	}  //Loop for materials

}

void renderModel(const aiNode* node, Model* model)
{
	const aiScene* scene = model->scene;
	aiMatrix4x4 m = node->mTransformation;
	aiMesh* mesh;
	aiFace* face;
	aiMaterial* mtl;
	aiColor4D diffuse;
	int meshIndex, materialIndex;
	int texNum = 0;

	aiTransposeMatrix4(&m);   //Convert to column-major order
	glPushMatrix();
	glMultMatrixf((float*)&m);   //Multiply by the transformation matrix for this node

	// Draw all meshes assigned to this node
	for (int n = 0; n < node->mNumMeshes; n++)
	{
		meshIndex = node->mMeshes[n];          //Get the mesh indices from the current node
		mesh = scene->mMeshes[meshIndex];    //Using mesh index, get the mesh object

		materialIndex = mesh->mMaterialIndex;  //Get material index attached to the mesh
		mtl = scene->mMaterials[materialIndex];

		if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))  //Get material colour from model
		{
			glColor4f(diffuse.r, diffuse.g, diffuse.b, 1.0);
		}


		if (mesh->HasTextureCoords(0))
		{
			aiString path;
			const char* cpath;
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, model->textureIds[materialIndex]);
		}
		else
			glDisable(GL_TEXTURE_2D);

		//Get the polygons from each mesh and draw them
		for (int k = 0; k < mesh->mNumFaces; k++)
		{
			face = &mesh->mFaces[k];
			GLenum face_mode;

			switch(face->mNumIndices)
			{
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);
			for(int i = 0; i < face->mNumIndices; i++) {
				int vertexIndex = face->mIndices[i]; 
				if (mesh->HasTextureCoords(0))
					glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, mesh->mTextureCoords[0][vertexIndex].y);
				if (mesh->HasNormals())
					glNormal3fv(&mesh->mNormals[vertexIndex].x);
				glVertex3fv(&mesh->mVertices[vertexIndex].x);
			}
			glEnd();
		}
	}

	// Draw all children
	for (int i = 0; i < node->mNumChildren; i++)
		renderModel(node->mChildren[i], model);

	glPopMatrix();
}