#include"geometry.h"
#include<vector>


Geometry::Geometry() {

}

Geometry::~Geometry() {
	if(mVao!=0)
	{
		glDeleteVertexArrays(1, &mVao);
	}
	if (mPosVbo != 0) {
		glDeleteBuffers(1, &mPosVbo);
	}
	if (mUvVbo != 0) {
		glDeleteBuffers(1, &mUvVbo);
	}
	if (mEbo != 0) {
		glDeleteBuffers(1, &mEbo);
	}
}

Geometry* Geometry::createBox(float size) {
	Geometry* geometry = new Geometry();
	geometry->mIndicesCount = 36;

	float halfSize = size / 2.0f;

	float positions[] = {
		

	// 右面 (16-19)
	halfSize, -halfSize, -halfSize,1,0,0,
	halfSize, halfSize,  -halfSize,1,0,0,
	halfSize,  halfSize, halfSize,1,0,0,
	halfSize,  -halfSize,  halfSize,1,0,0,

	// 左面 (20-23)
	-halfSize, -halfSize, -halfSize,-1,0,0,
	-halfSize, -halfSize,  halfSize,-1,0,0,
	-halfSize,  halfSize, halfSize,-1,0,0,
	-halfSize,  halfSize,  -halfSize,-1,0,0,

	// 顶面 (8-11)
	-halfSize,  halfSize, -halfSize,0,1,0,
	-halfSize,  halfSize, halfSize,0,1,0,
	halfSize,  halfSize,  halfSize,0,1,0,
	halfSize,  halfSize,  -halfSize,0,1,0,

	// 底面 (12-15)
	-halfSize, -halfSize, -halfSize,0,-1,0,
	halfSize, -halfSize, -halfSize,0,-1,0,
	halfSize, -halfSize,  halfSize,0,-1,0,
	-halfSize, -halfSize,  halfSize,0,-1,0,

	// 背面 (4-7)
	-halfSize, -halfSize,  -halfSize,0,0,-1,
	-halfSize,  halfSize,  -halfSize,0,0,-1,
	halfSize,  halfSize,  -halfSize,0,0,-1,
	halfSize, -halfSize,  -halfSize,0,0,-1,

	// 正面 (0-3)
	-halfSize, -halfSize, halfSize,0,0,1,
	halfSize, -halfSize, halfSize,0,0,1,
	halfSize,  halfSize, halfSize,0,0,1,
	-halfSize,  halfSize, halfSize,0,0,1,

	
	};

	float uvs[]{
		// 正面
 0.0f, 0.0f,
 1.0f, 0.0f,
 1.0f, 1.0f,
 0.0f, 1.0f,

 // 背面
 1.0f, 0.0f,
 0.0f, 0.0f,
 0.0f, 1.0f,
 1.0f, 1.0f,

 // 顶面
 0.0f, 1.0f,
 1.0f, 1.0f,
 1.0f, 0.0f,
 0.0f, 0.0f,

 // 底面
 1.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 0.0f,
 1.0f, 0.0f,

 // 左面
 1.0f, 0.0f,
 1.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 0.0f,

 // 右面
 0.0f, 0.0f,
 1.0f, 0.0f,
 1.0f, 1.0f,
 0.0f, 1.0f,
	};

	unsigned int indices[]{
		// 正面
	   0, 1, 2,
	   2, 3, 0,
	   // 背面
	   4, 5, 6,
	   6, 7, 4,
	   // 顶面
	   8, 9, 10,
	   10, 11, 8,
	   // 底面
	   12, 13, 14,
	   14, 15, 12,
	   // 右面
	   16, 17, 18,
	   18, 19, 16,
	   //左 面
	   20, 21, 22,
	   22, 23, 20,
	};

	//2 VBO创建
	GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);


	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	//3 EBO创建
	glGenBuffers(1, &geometry->mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//4 VAO创建
	glGenVertexArrays(1, &geometry->mVao);
	glBindVertexArray(geometry->mVao);

	//5 绑定vbo ebo 加入属性描述信息
//5.1 加入位置属性描述信息
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3*sizeof(float)));

	//5.3 加入uv属性描述数据
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	//5.4 加入ebo到当前的vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

	glBindVertexArray(0);
	return geometry;
}



Geometry* Geometry::createSphere(float radius) {
	Geometry* geometry = new Geometry();
	//目的：1 位置 2uv 3索引
	//1 主要变量声明
	std::vector<GLfloat>positions{};
	std::vector<GLfloat>uvs{};
	std::vector<GLuint>indices{};


	//2 通过两层循环（纬线在外，经线在内）->位置、uv
	int numLatLines = 60;//纬线
	int numLongLines = 60;//经线


	for (int i = 0; i <= numLatLines; i++) 
		for (int j = 0; j <= numLongLines; j++) {
			float phi = i * glm::pi<float>() / numLatLines;
			float theta = j * 2 * glm::pi<float>() / numLongLines;

			float y = radius * cos(phi);
			float x = radius * sin(phi) * cos(theta);
			float z = radius * sin(phi) * sin(theta);

			positions.push_back(x);
			positions.push_back(y);
			positions.push_back(z);

			float u = 1.0 - (float)j / (float)numLongLines;
			float v = 1.0 - (float)i / (float)numLatLines;

			uvs.push_back(u);
			uvs.push_back(v);
		}

		// 3 通过两层循环（这里没有=号）->顶点索引
		for (int i = 0; i < numLatLines; i++) 
			for (int j = 0; j < numLongLines; j++) {
				int p1 = i * (numLongLines + 1) + j;
				int p2 = p1 + numLongLines + 1;
				int p3 = p1 + 1;
				int p4 = p2 + 1;

				indices.push_back(p1);
				indices.push_back(p2);
				indices.push_back(p3);

				indices.push_back(p3);
				indices.push_back(p2);
				indices.push_back(p4);


			}
		
		//4 生成vbo与vao

		GLuint& posVbo = geometry->mPosVbo, &uvVbo = geometry->mUvVbo;
		glGenBuffers(1, &posVbo);
		glBindBuffer(GL_ARRAY_BUFFER, posVbo);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);


		glGenBuffers(1, &uvVbo);
		glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

		//3 EBO创建
		glGenBuffers(1, &geometry->mEbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

		//4 VAO创建
		glGenVertexArrays(1, &geometry->mVao);
		glBindVertexArray(geometry->mVao);

		//5 绑定vbo ebo 加入属性描述信息
	//5.1 加入位置属性描述信息
		glBindBuffer(GL_ARRAY_BUFFER, posVbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);


		//5.3 加入uv属性描述数据
		glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

		//5.4 加入ebo到当前的vao
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

		glBindVertexArray(0);

		geometry->mIndicesCount = indices.size();

		return geometry;



	}

