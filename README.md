### To build server on linux:
1. Navigate to sk2-project/server.
2. Create folder "build".
3. Run "cmake .."
4. Run "make"
5. At this point, an executable named "server" should be present in the build folder.
6. To start the server on port X Run "./server X"

___

### To build client on linux:
1. Navigate to sk2-project/client.
2. Create folder "build".
3. Run "/opt/Qt/6.4.2/gcc_64/bin/qmake ../client.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug"

IMPORTANT: "/opt/Qt/6.4.2/gcc_64/bin/qmake" is an example location and you should change it to match yours.

4. Run "make"
5. At this point, an executable named "client" should be present in the build folder.
6. To start the client Run "./client"

#### Alternative client linux build
1. Open the project with QtCreator and hit build and run.
