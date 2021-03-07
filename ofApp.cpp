#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	float phi_deg_step = 6;
	float theta_deg_step = 6;

	int R_start = (ofGetFrameNum() * 4) % 350 + 30;
	for (int R_param = R_start; R_param < R_start + 350; R_param += 80) {

		int R = R_param % 350 + 30;
		float r = R * 0.13;

		auto rotation_y = glm::rotate(glm::mat4(), ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.001), 0, 1, PI * -5, PI * 5), glm::vec3(0, 1, 0));
		auto rotation_x = glm::rotate(glm::mat4(), ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.001), 0, 1, PI * -5, PI * 5), glm::vec3(1, 0, 0));

		for (float phi_deg = 0; phi_deg < 360; phi_deg += phi_deg_step) {

			for (float theta_deg = 0; theta_deg < 360; theta_deg += theta_deg_step) {

				float threshold = ofMap(R, 30, 350, 0, 1);
				auto noise_value = ofNoise(glm::vec3(this->make_point(300, 80, theta_deg, phi_deg) * 0.005));
				if (noise_value < threshold) { continue; }

				auto noise_1 = ofNoise(glm::vec3(this->make_point(300, 80, theta_deg - theta_deg_step, phi_deg) * 0.005));
				auto noise_2 = ofNoise(glm::vec3(this->make_point(300, 80, theta_deg, phi_deg + phi_deg_step) * 0.005));
				auto noise_3 = ofNoise(glm::vec3(this->make_point(300, 80, theta_deg, phi_deg - phi_deg_step) * 0.005));
				auto noise_4 = ofNoise(glm::vec3(this->make_point(300, 80, theta_deg + theta_deg_step, phi_deg) * 0.005));

				auto index = this->face.getNumVertices();
				vector<glm::vec3> vertices;

				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg - theta_deg_step * 0.5, phi_deg - phi_deg_step * 0.5)));
				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg + theta_deg_step * 0.5, phi_deg - phi_deg_step * 0.5)));
				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg - theta_deg_step * 0.5, phi_deg + phi_deg_step * 0.5)));
				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg + theta_deg_step * 0.5, phi_deg + phi_deg_step * 0.5)));

				for (auto& vertex : vertices) {

					vertex = glm::vec4(vertex, 0) * rotation_y * rotation_x;
				}

				this->face.addVertices(vertices);

				this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 3);
				this->face.addIndex(index + 0); this->face.addIndex(index + 3); this->face.addIndex(index + 2);

				if (noise_1 < threshold) {

					this->frame.addVertex(vertices[0]);
					this->frame.addVertex(vertices[2]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}

				if (noise_2 < threshold) {

					this->frame.addVertex(vertices[2]);
					this->frame.addVertex(vertices[3]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}

				if (noise_3 < threshold) {

					this->frame.addVertex(vertices[0]);
					this->frame.addVertex(vertices[1]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);
				}

				if (noise_4 < threshold) {

					this->frame.addVertex(vertices[1]);
					this->frame.addVertex(vertices[3]);

					this->frame.addIndex(this->frame.getNumVertices() - 1);
					this->frame.addIndex(this->frame.getNumVertices() - 2);

				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	ofSetColor(255);
	this->frame.draw();

	ofSetColor(39);
	this->face.draw();

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}


//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}