//#include "../include/glm/glm.hpp"
//#include "../include/glm/gtc/matrix_transform.hpp"
//#include "Input.hpp"
//
////GLuint currentProgram, blitProgram, debugProgram, depthProgram, simpleProgram, shadowProgram, bgfxProgram, shadowMaskProgram;
////GLuint rttFramebuffer, rttTexture, depthTexture, shadowMaskTexture;
////int rttFramebuffer_width, rttFramebuffer_height;
//////int depthFramebuffer_width, depthFramebuffer_height;
////GLuint ltc_mat_texture, ltc_mag_texture;
////GLuint buffer, depthBuffer, lightRectBuffer, teapotBuffer, floorRectBuffer, shadowMaskBuffer;
//
//GLuint location(const GLchar* u)
//{
//	// FIXED PROGRAM
//	return glGetUniformLocation(currentProgram, u);
//}
//
//void RenderSceneGeometryWithShadowMap()
//{
//	GLuint currentProgram = debugProgram;
//
//	glUseProgram(currentProgram);
//
//	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
//
//	glDisable(GL_CULL_FACE);
//
//	/*glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
//
//	//screenshot_ppm_Depth("output.ppm", 512, 512, pixels, depthTexture);
//
//	glm::vec3 Camera_Pos;
//	/*cameraPitch = -73;
//
//	cameraYaw = -90;*/
//	cameraDistance = 100;
//	Camera_Pos[0] = cameraDistance * glm::sin(glm::radians(cameraPitch)) * glm::cos(glm::radians(cameraYaw));
//	Camera_Pos[1] = cameraDistance * glm::cos(glm::radians(cameraPitch));
//	Camera_Pos[2] = cameraDistance * glm::sin(glm::radians(cameraPitch)) * glm::sin(glm::radians(cameraYaw));
//
//	GLuint vertexPositionLocation = glGetAttribLocation(currentProgram, "position");
//
//	//glm::vec3 cameraPos = glm::vec3(0, 3, -3);
//	glm::vec3 cameraPos = Camera_Pos;
//	//glm::vec3 cameraPos = glm::vec3(0, 10, -6); // another view for debugging
//
//	//glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(LightCenter.x, cameraPos.y, LightCenter.z), glm::vec3(0, 1, 0));
//	glm::mat4 view = getViewMatrix();
//	glUniformMatrix4fv(glGetUniformLocation(currentProgram, "view"), 1, GL_FALSE, &view[0][0]);
//
//	//glm::mat4 proj = glm::perspective(glm::radians(45.0), 1.0, 0.0008, 1000.0);
//	glm::mat4 proj = getProjectionMatrix();
//	glUniformMatrix4fv(glGetUniformLocation(currentProgram, "proj"), 1, GL_FALSE, &proj[0][0]);
//
//	//float w = 14.35;
//	float w = 20.0;
//	//glm::mat4 proj_depth = glm::perspective(glm::radians(45.0), 1.0, 0.0008, 1000.0); // glm::ortho<float>(-w, w, -w, w, -1000, 2000);
//	glm::mat4 proj_depth = glm::ortho<float>(-w, w, -w, w, -1000, 2000);
//
//	glm::mat4 view_depth = glm::lookAt(LightCenter, LightCenter + glm::vec3(0.0, -0.15, -1.0), glm::vec3(0, 1, 0));
//	//glm::mat4 view_depth = glm::lookAt(LightCenter, glm::vec3(0,0,-8), glm::vec3(0, 1, 0));
//	glUniformMatrix4fv(glGetUniformLocation(currentProgram, "proj_depth"), 1, GL_FALSE, &proj_depth[0][0]);
//	glUniformMatrix4fv(glGetUniformLocation(currentProgram, "view_depth"), 1, GL_FALSE, &view_depth[0][0]);
//
//	//glm::mat4 proj_depth = glm::perspective(glm::radians(45.0), 1.0, 0.0008, 1000.0);//glm::ortho<float>(-10, 10, -10, 10, -10, 20);
//	//glm::mat4 view_depth = glm::lookAt(LightCenter, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 1, 0));
//	//glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "proj_depth"), 1, GL_FALSE, &proj_depth[0][0]);
//	//glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "view_depth"), 1, GL_FALSE, &view_depth[0][0]);
//
//	glActiveTexture(0);
//	glBindTexture(GL_TEXTURE_2D, depthTexture);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
//	glUniform1i(glGetUniformLocation(currentProgram, "shadowMap"), depthTexture);
//
//	glUniform1f(glGetUniformLocation(currentProgram, "bias"), shadowBias);
//
//	glm::mat4 model(1.0f);
//
//	/*
//	*	Draw the Floor
//	*/
//
//	model = glm::mat4(1.0f);
//	model = glm::translate(model, glm::vec3(0, 0, 0));
//	model = glm::scale(model, glm::vec3(1,1,1) * 10.1f);
//
//	glUniformMatrix4fv(glGetUniformLocation(currentProgram, "model"), 1, GL_FALSE, &model[0][0]);
//	glUniform3f(glGetUniformLocation(currentProgram, "debugColor"), 0.0, 1.0, 0.0);
//	glBindBuffer(GL_ARRAY_BUFFER, floorRectBuffer);
//	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, false, 0, 0);
//	glEnableVertexAttribArray(vertexPositionLocation);
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//
//	/*
//	*	Draw the Teapot
//	*/
//	//glEnable(GL_BLEND);
//	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	glm::vec3 teapotPosition = glm::vec3(0, 0, 0);
//	//glm::vec3 teapotPosition = glm::vec3(0, 0, 50);
//	model = glm::mat4(1.0f);
//	// model = glm::translate(model, glm::vec3(0, 0, 15));
//	model = glm::translate(model, teapotPosition);
//	model = glm::scale(model, glm::vec3(1, 1, 1) * 2.6f);
//
//	glUniformMatrix4fv(glGetUniformLocation(currentProgram, "model"), 1, GL_FALSE, &model[0][0]);
//
//	glUniform3f(glGetUniformLocation(currentProgram, "debugColor"), 0.0, 0.0, 1.0);
//	glBindBuffer(GL_ARRAY_BUFFER, teapotBuffer);
//	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, false, 0, 0);
//	glEnableVertexAttribArray(vertexPositionLocation);
//	glDrawArrays(GL_TRIANGLES, 0, teapot.position.size() / 3);
//
//	/*
//	*	Draw Another Teapot
//	*/
//	teapotPosition = glm::vec3(3, 0, -5);
//	model = glm::mat4(1.0f);
//	model = glm::translate(model, teapotPosition);
//	model = glm::scale(model, glm::vec3(1, 1, 1) * 3.0f);
//	glUniformMatrix4fv(glGetUniformLocation(currentProgram, "model"), 1, GL_FALSE, &model[0][0]);
//	glUniform3f(glGetUniformLocation(currentProgram, "debugColor"), 0.0, 1.0, 1.0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, teapotBuffer);
//	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, false, 0, 0);
//	glEnableVertexAttribArray(vertexPositionLocation);
//	//glDrawArrays(GL_TRIANGLES, 0, teapot.position.size() / 3);
//
//	/*
//	*	Draw the Light Rect
//	*/
//
//	model = glm::mat4(1.0f);
//	// Light Center = vec3(0, 6, 32);
//	model = glm::translate(model, LightCenter);
//
//
//	/*
//	vec3 rotation_y(vec3 v, float a){return vec3(v.x*cos(a) + v.z*sin(a), v.y, -v.x*sin(a) + v.z*cos(a));}
//	vec3 rotation_z(vec3 v, float a){return vec3(v.x*cos(a) - v.y*sin(a), v.x*sin(a) + v.y*cos(a), v.z);}
//	vec3 rotation_yz(vec3 v, float ay, float az){return rotation_z(rotation_y(v, ay), az);}
//	*/
//
//	/*
//	glm::vec3 v = glm::vec3(0, 1, 0);
//	v = glm::vec3(v.x*cos(roty) + v.z*sin(roty), v.y, -v.x*sin(roty) + v.z*cos(roty));
//	glm::vec3 rotAxisAfterYRotated = glm::vec3(v.x*cos(roty) - v.y*sin(roty), v.x*sin(roty) + v.y*cos(roty), v.z);
//	model = glm::rotate(model, -roty * 2.0f * 3.14f, v);
//
//	v = glm::vec3(0, 0, 1);
//	v = glm::vec3(v.x*cos(rotz) - v.y*sin(rotz), v.x*sin(rotz) + v.y*cos(rotz), v.z);
//	*/
//
//	model = glm::rotate(model, -roty * 2.0f * 3.14f, glm::vec3(0, 1, 0));
//	model = glm::rotate(model, -rotz * 2.0f * 3.14f, glm::vec3(0, 0, 1));
//	model = glm::scale(model, glm::vec3(width * 0.5, height * 0.5, 1));
//
//	glUniformMatrix4fv(glGetUniformLocation(currentProgram, "model"), 1, GL_FALSE, &model[0][0]);
//	glUniform3f(glGetUniformLocation(currentProgram, "debugColor"), 1.0, 0.0, 0.0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, lightRectBuffer);
//	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, false, 0, 0);
//	glEnableVertexAttribArray(vertexPositionLocation);
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//
//	glDisableVertexAttribArray(vertexPositionLocation);
//	glDisable(GL_BLEND);
//
//	// Debug Draw
//	/*glPushMatrix();
//	glLoadIdentity();
//	glTranslatef(LightCenter.x, LightCenter.y, LightCenter.z);
//	glutSolidCylinder(1, 2, 5, 5);
//	glPopMatrix();*/
//
//	/*
//	*	Draw the Super Big Light Rect
//	*/
//
//	model = glm::mat4(1.0f);
//	model = glm::translate(model, glm::vec3(0, 3, -35));
//	model = glm::scale(model, glm::vec3(width * 100, height * 100, 1));
//
//	glUniformMatrix4fv(glGetUniformLocation(currentProgram, "model"), 1, GL_FALSE, &model[0][0]);
//	glUniform3f(glGetUniformLocation(currentProgram, "debugColor"), 1, 1, 1);
//
//	glBindBuffer(GL_ARRAY_BUFFER, lightRectBuffer);
//	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, false, 0, 0);
//	glEnableVertexAttribArray(vertexPositionLocation);
//	//glDrawArrays(GL_TRIANGLES, 0, 6);
//}
//
//void RenderSceneGeometry()
//{
//	glUseProgram(simpleProgram);
//
//	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
//
//	glDisable(GL_CULL_FACE);
//
//	/*glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
//
//	/*glm::vec3 Camera_Pos;
//	Camera_Pos[0] = cameraDistance * glm::sin(glm::radians(cameraPitch)) * glm::cos(glm::radians(cameraYaw));
//	Camera_Pos[1] = cameraDistance * glm::cos(glm::radians(cameraPitch));
//	Camera_Pos[2] = cameraDistance * glm::sin(glm::radians(cameraPitch)) * glm::sin(glm::radians(cameraYaw));*/
//
//	GLuint vertexPositionLocation = glGetAttribLocation(simpleProgram, "position");
//
//	glm::vec3 cameraPos = glm::vec3(0, 3, -3);
//	//glm::vec3 cameraPos = glm::vec3(0, 10, -6); // another view for debugging
//
//	glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(LightCenter.x, cameraPos.y, LightCenter.z), glm::vec3(0, 1, 0));
//	glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "view"), 1, GL_FALSE, &view[0][0]);
//
//	//glm::mat4 proj = glm::perspective(glm::radians(45.0), 1.0, 0.0001, 1000.0);
//	glm::mat4 proj = glm::perspective(glm::radians(45.0), 1.0, 0.0008, 1000.0);
//	//glm::mat4 proj = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
//	glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "proj"), 1, GL_FALSE, &proj[0][0]);
//
//	glm::mat4 proj_depth = glm::perspective(glm::radians(45.0), 1.0, 0.0008, 1000.0);//glm::ortho<float>(-10, 10, -10, 10, -10, 20);
//	glm::mat4 view_depth = glm::lookAt(LightCenter, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 1, 0));
//	glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "proj_depth"), 1, GL_FALSE, &proj_depth[0][0]);
//	glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "view_depth"), 1, GL_FALSE, &view_depth[0][0]);
//
//	glm::mat4 model(1.0f);
//
//	/*
//	*	Draw the Floor
//	*/
//
//	model = glm::mat4(1.0f);
//	model = glm::translate(model, glm::vec3(0, 0, 0));
//	model = glm::scale(model, glm::vec3(50, 1, 90));
//
//	glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "model"), 1, GL_FALSE, &model[0][0]);
//	glUniform3f(glGetUniformLocation(simpleProgram, "debugColor"), 0.0, 1.0, 0.0);
//	glBindBuffer(GL_ARRAY_BUFFER, floorRectBuffer);
//	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, false, 0, 0);
//	glEnableVertexAttribArray(vertexPositionLocation);
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//
//	/*
//	*	Draw the Teapot
//	*/
//	//glEnable(GL_BLEND);
//	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	model = glm::mat4(1.0f);
//	// model = glm::translate(model, glm::vec3(0, 0, 15));
//	model = glm::translate(model, glm::vec3(10, 0, 33));
//	model = glm::scale(model, glm::vec3(1, 1, 1) * 5.0f);
//
//	glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "model"), 1, GL_FALSE, &model[0][0]);
//
//	glUniform3f(glGetUniformLocation(simpleProgram, "debugColor"), 0.0, 0.0, 1.0);
//	glBindBuffer(GL_ARRAY_BUFFER, teapotBuffer);
//	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, false, 0, 0);
//	glEnableVertexAttribArray(vertexPositionLocation);
//	glDrawArrays(GL_TRIANGLES, 0, teapot.position.size() / 3);
//
//	/*
//	*	Draw the Light Rect
//	*/
//
//	model = glm::mat4(1.0f);
//	// Light Center = vec3(0, 6, 32);
//	model = glm::translate(model, LightCenter);
//
//
//	/*
//	vec3 rotation_y(vec3 v, float a){return vec3(v.x*cos(a) + v.z*sin(a), v.y, -v.x*sin(a) + v.z*cos(a));}
//	vec3 rotation_z(vec3 v, float a){return vec3(v.x*cos(a) - v.y*sin(a), v.x*sin(a) + v.y*cos(a), v.z);}
//	vec3 rotation_yz(vec3 v, float ay, float az){return rotation_z(rotation_y(v, ay), az);}
//	*/
//
//	/*
//	glm::vec3 v = glm::vec3(0, 1, 0);
//	v = glm::vec3(v.x*cos(roty) + v.z*sin(roty), v.y, -v.x*sin(roty) + v.z*cos(roty));
//	glm::vec3 rotAxisAfterYRotated = glm::vec3(v.x*cos(roty) - v.y*sin(roty), v.x*sin(roty) + v.y*cos(roty), v.z);
//	model = glm::rotate(model, -roty * 2.0f * 3.14f, v);
//
//	v = glm::vec3(0, 0, 1);
//	v = glm::vec3(v.x*cos(rotz) - v.y*sin(rotz), v.x*sin(rotz) + v.y*cos(rotz), v.z);
//	*/
//
//	model = glm::rotate(model, -roty * 2.0f * 3.14f, glm::vec3(0, 1, 0));
//	model = glm::rotate(model, -rotz * 2.0f * 3.14f, glm::vec3(0, 0, 1));
//	model = glm::scale(model, glm::vec3(width * 0.5, height * 0.5, 1));
//
//	glUniformMatrix4fv(glGetUniformLocation(simpleProgram, "model"), 1, GL_FALSE, &model[0][0]);
//	glUniform3f(glGetUniformLocation(simpleProgram, "debugColor"), 1.0, 0.0, 0.0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, lightRectBuffer);
//	glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, false, 0, 0);
//	glEnableVertexAttribArray(vertexPositionLocation);
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//
//	glDisableVertexAttribArray(vertexPositionLocation);
//	glDisable(GL_BLEND);
//}
//
//void RenderShadowMap()
//{
//	/*glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_GREATER);*/
//
//	// We don't use bias in the shader, but instead we draw back faces, 
//	// which are already separated from the front faces by a small distance 
//	// (if your geometry is made this way)
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles
//
//	glBindFramebuffer(GL_FRAMEBUFFER, depthBuffer);
//	// glBindFramebuffer(GL_FRAMEBUFFER, NULL);
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glUseProgram(depthProgram);
//
//	//glm::vec3 cameraPos = glm::vec3(0.0, 3.0, -3);
//	//glm::vec3 cameraPos = glm::vec3(0.0, 3.0, 6);
//
//	glm::mat4 model(1.0f);
//	glm::mat4 view(1.0f);
//	glm::mat4 proj(1.0f);
//	glm::mat4 MVP(1.0f);
//
//	//view = glm::lookAt(cameraPos, glm::vec3(LightCenter.x, cameraPos.y, LightCenter.z), glm::vec3(0, 1, 0));
//
//	//float w = 14.35;
//	float w = 20.0f;
//	proj = glm::ortho<float>(-w, w, -w, w, -1000, 2000);
//	//proj = glm::perspective(glm::radians(45.0), 1.0, 0.0008, 1000.0);
//
//	//glm::mat4 proj_depth = glm::perspective(glm::radians(45.0), 1.0, 0.0008, 1000.0);//glm::ortho<float>(-10, 10, -10, 10, -10, 20);
//	//view = glm::lookAt(LightCenter, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
//	view = glm::lookAt(LightCenter, LightCenter + glm::vec3(0.0, -0.15, -1.0), glm::vec3(0, 1, 0));
//	//view_depth = glm::lookAt(LightCenter, glm::vec3(10, 0, 33 + roty), glm::vec3(0, 1, 0));
//
//	//view = view_depth;
//
//	/*
//	*	Draw the Floor
//	*/
//
//	// Why does floor not display?
//	model = glm::mat4(1.0f);
//	model = glm::translate(model, glm::vec3(0, 0, 0));
//	model = glm::scale(model, glm::vec3(50, 1, 90));
//	MVP = proj * view * model;
//	glUniformMatrix4fv(glGetUniformLocation(depthProgram, "MVP"), 1, GL_FALSE, &model[0][0]);
//	glBindBuffer(GL_ARRAY_BUFFER, floorRectBuffer);
//	glVertexAttribPointer(glGetAttribLocation(depthProgram, "position"), 3, GL_FLOAT, false, 0, 0);
//	glEnableVertexAttribArray(glGetAttribLocation(depthProgram, "position"));
//	//glDrawArrays(GL_TRIANGLES, 0, 6);
//
//	/*
//	*	Draw the Teapot
//	*/
//
//	glm::vec3 teapotPosition = glm::vec3(0, 0, 0);// +LightCenter;
//												  //glm::vec3 teapotPosition = glm::vec3(0, 0, -8);
//												  //glm::vec3 teapotPosition = glm::vec3(0, 0, 50);
//	model = glm::mat4(1.0f);
//	model = glm::translate(model, teapotPosition);
//	model = glm::scale(model, glm::vec3(1, 1, 1) * 5.0f);
//	//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1, 0, 0) );
//
//	MVP = proj * view * model;
//
//	glUniformMatrix4fv(glGetUniformLocation(depthProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);
//
//	glBindBuffer(GL_ARRAY_BUFFER, teapotBuffer);
//	glVertexAttribPointer(glGetAttribLocation(depthProgram, "position"), 3, GL_FLOAT, false, 0, 0);
//	glEnableVertexAttribArray(glGetAttribLocation(depthProgram, "position"));
//	glDrawArrays(GL_TRIANGLES, 0, teapot.position.size() / 3);
//
//	glDisableVertexAttribArray(glGetAttribLocation(depthProgram, "position"));
//
//	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
//	//screenshot_ppm_DepthPow("output.ppm", 512, 512, pixels, depthTexture, 1);
//	glUseProgram(NULL);
//
//
//}
//
//void DebugRender()
//{
//	glUseProgram(debugProgram);
//
//	//GLuint tex = rttTexture;
//	GLuint tex = depthTexture;
//
//	//screenshot_ppm_DepthPow("output.ppm", 512, 512, pixels, depthTexture, 100000);
//	//screenshot_ppm_DepthPow("output.ppm", 512, 512, pixels, depthTexture, 1);
//
//	// Set textures
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, tex);
//
//	//glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, pixels);
//
//	//float sum = 0;
//	//for (int i = 0; i < 512 * 512; ++i)
//	//{
//	//	sum += pixels[i];
//	//}
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
//
//	glUniform1i(glGetUniformLocation(debugProgram, "tex"), 0);
//	glUniform2f(glGetUniformLocation(debugProgram, "resolution"), parameters.screenWidth, parameters.screenHeight);
//
//	GLuint vertexPositionLocation = 0;
//	/* line 645 */
//	glBindBuffer(GL_ARRAY_BUFFER, buffer);
//	// Blit pass
//	glEnableVertexAttribArray(vertexPositionLocation);
//	glVertexAttribPointer(vertexPositionLocation, 2, GL_FLOAT, false, 0, 0);
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//
//	glDisableVertexAttribArray(vertexPositionLocation);
//	glBindBuffer(GL_ARRAY_BUFFER, NULL);
//
//}
//
