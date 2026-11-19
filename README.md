# 노노그램 함수 1, 2 단위 테스트

현재 테스트는 생성한 노노그램 구조체/함수 중 다음 두 함수만 대상으로 합니다.

1. `nonogram_init` — `tests/test_nonogram_init.c`
2. `nonogram_is_valid_position` — `tests/test_nonogram_is_valid_position.c`

각 테스트를 따로 실행하려면 저장소 루트에서 다음 명령을 사용합니다.

```bash
make test-init
make test-valid-position
```

둘 다 실행하려면 다음을 실행합니다.

```bash
make test
```
