
#include "charater.h"

bool replace_links(aiString name, Model models[]) {
	float r = 1;
	if (name == aiString("Chest")) {
		chest();
		return true;
	} else if (name == aiString("lowerback")) {
		middle();
		return true;
	}  else if (name == aiString("Head")) {
		head();
		glDisable(GL_TEXTURE_2D);
		glTranslatef(0, 0.25, -2);
		glRotatef(45, 1, 0, 0);
		glScalef(0.11, 0.11, 0.11);
		renderModel(models[1].scene->mRootNode, &models[1], true);
		glEnable(GL_TEXTURE_2D);
		return true;
	} else if (name == aiString("LeftShoulder") || name == aiString("RightShoulder")) {
		shoulder();
		return true;
	} else if (name == aiString("RightHip")) {
		rightHip();
		return true;
	}  else if (name == aiString("LeftHip")) {
		leftHip();
		return true;
	} else if (name == aiString("LeftKnee")) {
		knee(true);
		return true;
	}  else if (name == aiString("RightKnee")) {
		knee(false);
		return true;
	}  else if (name == aiString("LeftElbow") || name == aiString("RightElbow")) {
		elbow();
		return true;
	} else if (name == aiString("lhand")) {
		hand();
		glPushMatrix();
			glScalef(15, 15, 15);
			glRotatef(70, 1, 0, 0);
			glRotatef(25, 0, 1, 0);
			renderModel(models[0].scene->mRootNode, &models[0], false);
		glPopMatrix();
		return true;
	} else if (name == aiString("rhand")) {
		hand();
		return true;
	} else if (name == aiString("LHipJoint") || name == aiString("RHipJoint")) {
		return true;
	} else if (name == aiString("RightAnkle")) {
		foot(1.5, false);
		return true;
	} else if (name == aiString("LeftAnkle")) {
		foot(1.5, true);
		return true;
	} else if (name == aiString("RightToe") || name == aiString("LeftToe")) {
		return true;
	}
	return false; 
}


void renderCharater(const aiNode* node, Charater charater, Model models[])
{
	const aiScene* scene = charater.scene;              				 	 	 	 	 	 
	aiMatrix4x4 m = node->mTransformation;
	aiMesh* mesh;
	aiFace* face;
	float materialCol[4] = { 1, 0, 1, 1 };
	int meshIndex;

	m.Transpose();   //Convert to column-major order
	glPushMatrix();
	glMultMatrixf((float*)&m);   //Multiply by the transformation matrix for this node

	//The scene graph for a skeleton contains at most one mesh per node
	//Skeleton meshes are always triangle meshes
	

	if(node->mNumMeshes > 0 && false) {

		if (false) {

			meshIndex = node->mMeshes[0];          //Get the mesh indices from the current node
			mesh = scene->mMeshes[meshIndex];    //Using mesh index, get the mesh object
			glColor4fv(materialCol);   //Default material colour

			//Draw the mesh in the current node
			for (int k = 0; k < mesh->mNumFaces; k++)
			{
				face = &mesh->mFaces[k];
				glBegin(GL_TRIANGLES);
				for (int i = 0; i < face->mNumIndices; i++) {
					int vertexIndex = face->mIndices[i];
					if (mesh->HasNormals())
						glNormal3fv(&mesh->mNormals[vertexIndex].x);
					glVertex3fv(&mesh->mVertices[vertexIndex].x);
				}
				glEnd();
			}
		}
	} else {
		replace_links(node->mName, models);
	}

	// Recursively draw all children of the current node
	for (int i = 0; i < node->mNumChildren; i++)
		renderCharater(node->mChildren[i], charater, models);
		

	glPopMatrix();
}