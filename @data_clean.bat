
::============================================================================
:: _saveディレクトリを削除
::============================================================================
pushd "%~dp0"
if exist data\_save (
	call bin\gb.exe data\_save
)
popd


@endlocal
