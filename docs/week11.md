# Week 11 회의록 (Nov. 11 ~ Nov. 17)

## 이번 주에 수행한 것
* 프로젝트 Repository의 대략적인 구조 정의 (README.md : Project Structure)
* 대략적인 소스 파일 구조 정의
  * menu.c - 메인메뉴 UI 및 메뉴 선택과 관련된 기능 구현
  * single/time_attack.c - 싱글 타임어택 모드 구현
  * single/endless.c - 싱글 무한 모드 구현
  * multi/ - 멀티플레이 구현 (미정)
  * util/scoring.c - 점수 계산과 관련된 utility 구현
  * util/string.c - 단어 관리를 위한 string utility 구현
* 원활환 협업을 위한 코딩 스타일(코딩 컨벤션) 정의 (README.md : Programming Style)
* 단어 데이터베이스로 쓸 리소스 정하기
  * 직접 파싱
  * dwyl/english-words (https://github.com/dwyl/english-words)
  * first20hours/google-10000-english (https://github.com/first20hours/google-10000-english)

## 문제점
없음

## 해야할 것
* 단어 리소스 가공
  * 등장할 단어의 최소, 최대 길이 정의 - 최소 2글자, 최대 ?글자
  * 단어 리스트에서 적절하지 않은 길이의 단어들 제거 (가공)

