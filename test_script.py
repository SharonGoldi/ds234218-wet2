from pathlib import Path
from os import system

if __name__ == '__main__':
    for i in range(15):
        test_path = 'tests/randtest{}.in'.format(i)
        test_out_path = 'tests/randtest{}.out'.format(i)
        my_out_path = 'tests/randtest{}.my'.format(i)

        print('RUNNING tests {}'.format(i))
        system('./wet2 < {} > {}'.format(test_path, my_out_path))
        print('DIFF for tests {} is: '.format(i))
        system('diff {} {}'.format(test_out_path, my_out_path))
