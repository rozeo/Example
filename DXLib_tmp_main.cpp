#include "DxLib.h"

void Draw();
void Ctrl();
void Init();
void KeyStack();
int CheckStateKey( unsigned char );
void ModeSwitcher();

int stateKey[256];

//�V�X�e���̏������n�͂���(�ق���ChangeWindowMode�Ƃ�
int SystemInit(){
	if( DXLib_Init() != 0 ) return -1;
	if( SetDrawScreen() != 0 ) return -2;
	return 0;
}

//while���[�v�̏����Ƃ��čs�킹��̂͂���
int MainProcess(){
	if( ProcessMassage() != 0 ) return -1;
	if( ClearDrawScreen() != 0 ) return -2;
	return 0;
}

//�ɗ͂����Ȃ��B
int WINAPI WinMain( HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nCmdShow ){
	if( SystemInit() != 0 ) return -1;

	Init();

	while( MainProcess() == 0 ){
		
		ModeSwitcher();
		Draw();
		
		ScreenFlip();
	}

	Finalize();
	DxLib_Init();
	return 0;
}

//�L�[���͂̎�t�B��{�m�[�^�b�`�B
void KeyStack(){
	char key[256];
	GethitKeyStateAll( key );
	for( int i = 0;i < 256;i++ ){
		if( key[i] == 0 ) stateKey[i] = 0;
		else			  stateKey[i]++;
	}
}

//�L�[�̓��͏�Ԃ��ĂԁB�Ȃ�ׂ�[x]�̂悤�Ȕz��Q�Ƃ͎g��Ȃ�
//������KEY_INPUT_~���w�肷��
int CheckStateKey( unsigned char Handle ){ return stateKey[ Handle ]; }

//�ϐ��̏������A�摜�̃��[�h�Ȃǂ̏������n�͂��ׂĂ���
void Init(){

}

//�o�b�N�O���E���h��������̔��i�v�I�ɕ\�����Â�����̂͂����B
void Draw(){

}

//�������̊J���A�n���h���̃����[�X�Ȃǂ̏I�������͂����B
void Finalize(){

}

//�Q�[���̃V�X�e�����������Ƃ���B
//ScreenFlip�͋L�q���Ȃ����d�v
//while�ɂ�鏈���X�g�b�v�n�͔񐄏��Aswitch�Ƀ��[�h�������ϐ��ŕ��򂳂��āA
//#define�ȂǂŃ��[�h�̎��ʎq���`����Ȃǂ��āA��{���i�ォ�牺�܂�while�Ȃǂ�
//�������[�v���Ȃ��B�j�ɂ���B
void ModeSwitcher(){

	/*��
	//gameMode : ���[�h�������ϐ��B
	switch( gameMode ){
	case MENU: //���j���[
	//�u���b�N�����ł����A�X�y�[�X�L�[�҂��������BCheckStateKey( KEY_INPUT_SPACE )�ő҂��󂯂�
	//�߂�l��1( CheckStateKey( KEY_INPUT_SPACE ) == 1 )�̂Ƃ���gameMode = MAIN�ŃQ�[���{�̕��ɂ���B
		~~~~�ȗ��B
		break;
	
	case MAIN: //�Q�[���{�̕�
	//�u���b�N�̕\���A�v���C���[�o�[�̑��암�B
	//�Q�[���I�[�o�[�����ɂȂ��gameMode = GAMEOVER��GAMEOVER��ʂɂ���B
		~~~~�ȗ��B
		break;

	case GAMEOVER://�Q�[���I�[�o�[���
		~~~~�ȗ�
		break;
	}

	*/

	//ScreenFlip() �͂����Ȃ��B

}
//switch�̂Ȃ��ŊO���̊֐����Ă�ł������肳����Ƃ��Ƃ��ƌ��₷���\�[�X���ł���B

//�C���f���g��Tab�L�[�ň��̋󔒂�������A������������B

/*��������F

	for( int i = 0;i < 6;i++ ){
		if( i == 1 ) printf( "ba-ka );
	}

	������������Ə����̗��ꂪ���݂₷���A�R�[�h�����₷���Ȃǂ̃����b�g������
*/