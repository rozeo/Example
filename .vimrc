" https://qiita.com/marrontan619/items/541a1374e1ac672977e6
set number
set title
set ambiwidth=double
set tabstop=4
set expandtab
set shiftwidth=4
set laststatus=2
set smartindent
set virtualedit=block
set whichwrap=b,s,[,],<,>
set backspace=indent,eol,start

"dein Scripts-----------------------------
if &compatible
  set nocompatible               " Be iMproved
endif

" Required:
set runtimepath+=$HOME/.cache/dein/repos/github.com/Shougo/dein.vim

" Required:
if dein#load_state('$HOME/.cache/dein')
  call dein#begin('$HOME/.cache/dein')

  " Let dein manage dein
  " Required:
  call dein#add('$HOME/.cache/dein/repos/github.com/Shougo/dein.vim')

  " Add or remove your plugins here:
  call dein#add('Shougo/neosnippet.vim')
  call dein#add('Shougo/neosnippet-snippets')

  call dein#load_toml('/home/rozeo/.vim/plugins.toml', {'lazy': 0})


  " You can specify revision/branch/tag.
  call dein#add('Shougo/deol.nvim', { 'rev': '01203d4c9' })

  " Required:
  call dein#end()
  call dein#save_state()
endif

" Required:
filetype plugin on
filetype plugin indent on
syntax enable

" If you want to install not installed plugins on startup.
if dein#check_install()
  call dein#install()
endif

"End dein Scripts-------------------------

let g:molokai_original = 1

let b:ale_linters = {'php': ['phpstan', 'php']}

autocmd FileType php setlocal omnifunc=phpactor#Complete

let g:deoplete#sources#go#gocode_binary = '/home/rozeo/go/bin/gocode'
let g:deoplete#sources#go#sort_class = ['package', 'func', 'type', 'var', 'const']

" 上部に表示される情報を非表示
let g:netrw_banner = 0
" 表示形式をTreeViewに変更
let g:netrw_liststyle = 3
" 左右分割を右側に開く
let g:netrw_altv = 1
" 分割で開いたときに85%のサイズで開く
let g:netrw_winsize = 80


" Airline setting
let g:airline_theme='alduin'

if !exists('g:airline_symbols')
  let g:airline_symbols = {}
endif

let g:ale_php_phpstan_level=5

" unicode symbols
" let g:airline_left_sep = '»'
" let g:airline_left_sep = '▶'
" let g:airline_right_sep = '«'
" let g:airline_right_sep = '◀'

noremap sh <C-w>h
noremap sj <C-w>j
noremap sk <C-w>k
noremap sl <C-w>l
noremap <S-h> ^
noremap <S-l> $

" execute php-cs
autocmd BufWritePost *.php silent! call PhpCsFixerFixFile()
