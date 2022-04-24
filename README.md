# Ply Parser

convert *.ply to *.ply

## 알고리즘 동작 순서

1. 기존 ply 폴더를 순회하며 읽어들임 (이 때 순차적으로 읽는것이 아니라 특정 조건(나도 모름)에 따라 읽는듯..) <br/>
2. 불러온 ply 파일에서 x, y, z 값만 따로 쓸 거임 <br/>
3. 새롭게 ply를 쓸 것이기 때문에 properties에 x, y, z and timestamp를 추가해줘서 객체를 생성
4. 새로운 ply에 기존 파일의 x, y, z를 저장 후에 timestamp는 임의로 할당해줌 (e.g. timestamp += 0.1 ) <br/>

<br/>

`ct_icp/${dir_name}`에 있는 frame_####.ply 파일을 읽고, <br/>
`ct_icp_text/${dir_name}`에 그 결과값을 저장함 <br/>


## How to use

clone this repo. <br/>

~~~bash
$ git clone https://github.com/bigbigpark/PLY-PARSER.git
~~~

build and run

~~~bash
$ cd PLY-PARSER
$ mkdir build
$ cmake ../
$ make
~~~

Specify the folder <br/>

~~~bash
$ ./main sangam
~~~

## TODO
- [ ] ply file을 순회할 때 랜덤으로 읽는 문제 해결
- [ ] ${dir_name}에 폴더가 없을 때 생성하는 알고리즘 짜야함
- [ ] 생성한 ply 파일에 timestamp 값을 대입해서 저장해야함
- [ ] rosbag file을 바로 ply로 저장할 수 있도록 해야함
- [ ] 나중에 실시간으로 동작할 수 있게끔 인터페이스 코드를 짜야함
