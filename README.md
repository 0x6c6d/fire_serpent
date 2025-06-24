# fire_serpent

CS2 kernel hack that hooks the "NtOpenCompositionSurfaceSectionInfo" (dxgkrnl.sys) function. 
The custom driver (km) replaces the first 12 bytes of the "NtOpenCompositionSurfaceSectionInfo" with 
an address to a custom function. The custom function can read & write memory, find modules and processes.

<ins>before hooking</ins>
```
    dxgkrnl!NtOpenCompositionSurfaceSectionInfo: CFG
fffff805`715b8080 488bc4             mov     rax, rsp
fffff805`715b8083 4c894820           mov     qword ptr [rax+20h], r9
fffff805`715b8087 48894808           mov     qword ptr [rax+8], rcx
fffff805`715b808b 53                 push    rbx
fffff805`715b808c 57                 push    rdi
fffff805`715b808d 4154               push    r12
fffff805`715b808f 4156               push    r14
```

<ins>after hooking</ins>
```
    dxgkrnl!NtOpenCompositionSurfaceSectionInfo: CFG
fffff800`54fc8080 48b800c1aefc8c93ffff mov     rax, 0FFFF938CFCAEC100h
fffff800`54fc808a ffe0                 jmp     rax
```