; AutoHotKey， 通过快捷键ctrl+alt+c打开选中文件或文件夹所在的VS Code窗口
GetSelectedPath()
{
    global ClipSavedContent  ; Renamed to avoid conflict with built-in Clipboard variable
    ; 保存当前剪贴板内容
    ClipSavedContent := ClipboardAll
    ; 发送快捷键复制选中文件路径
    Send "^c"
    Sleep 500 ; 等待剪贴板更新
    ; 检查剪贴板内容是否为空或不符合路径格式
    if (Clipboard != "" && (InStr(Clipboard, ":") || InStr(Clipboard, "\")))  ; 检查是否包含路径分隔符
    {
        selectedPath := Clipboard
    }
    else
    {
        selectedPath := ""
    }
    ; 恢复剪贴板内容
    Clipboard := ClipSavedContent
    return selectedPath
}

^!c::
{
    ; 获取当前选中的文件或文件夹路径
    selectedPath := GetSelectedPath()
    if (selectedPath = "")
    {
        MsgBox "未选中文件或文件夹，或无法获取有效路径。"
        return
    }
    ; 启动 VS Code
    Run, %ComSpec% /C code "%selectedPath%", , Hide
}