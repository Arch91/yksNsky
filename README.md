<h1 align="center">yks'N'sky</h1>

Два байтсваппера сигнатур **Skyrim**

**ykstosky** — из BE в LE  
**skytoyks** — из LE в BE

Делались путём сравнения файлов _SKYRIM.ESM_ от PS3 и _Skyrim.esm_ от ПК Skyrim LE aka Oldrim. Могут содержать ошибки в каких-то сигнатурах, так что читайте комментарии исходников для той или иной сигнатуры, где я был уверен при сравнении, а где нет. Первоначально предназначались для сваппинга только самого файла _SKYRIM.ESM_, но может быть использовано и для других мастер-файлов и плагинов. Всё же, об ошибках вы предупреждены, а также учтите, что плагин/мастер-файл должен быть такой, который использует файлы локализации _.STRINGS .ILSTRINGS .DLSTRINGS_ (иначе получится так, что те сигнатуры включают в себя strings имён и текстов, и алгоритм свапа для них уже нужен будет другой...)

Используйте код как вам захочется, меня можете не спрашивать. Но, всё же, буду признателен, если при использовании вы, найдя ошибки в логике применения сваппинга для той или иной сигнатуры, уведомите меня об этом :D
<br><br><br>
ENG:  
Two signature byte-swappers for **Skyrim**

**ykstosky** — from BE to LE  
**skytoyks** — from LE to BE

These tools were created by comparing the _SKYRIM.ESM_ file from the PS3 version with the _Skyrim.esm_ file from the PC version of Skyrim LE aka Oldrim. The tools may contain inaccuracies/mistakes in certain signatures, so please read the comments in the source code for each specific signature (sorry, English-understanding guys, due to my defective English, it is quite complicated for me to translate their meaning precisely correct) to see where the mapping is guaranteed and where I had doubts. Originally designed exclusively for swapping the main SKYRIM.ESM file, they can also be used for other master files and plugins. However also, while you are warned about potential issues, please note that the target plugin or master file must be of that kind which uses external localization files _.STRINGS .ILSTRINGS .DLSTRINGS_ (otherwise, the text strings and names will remain embedded inside those signatures, which would require a different appropriate swapping algorithm...)

Feel free to use, modify, and distribute this code however you like — no need to ask the permission from me. Even so, if you use these tools and find any logical errors in how specific signatures are swapped, I would highly appreciate it if you could notify me about them :D
