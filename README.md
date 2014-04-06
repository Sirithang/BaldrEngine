Baldr Engine
============

Baldr engine is a PBR engine.
It contains an abstraction layer over graphics API.

*NOTE : It require AlfarMath to compile*

Design Philosophy
=================

The whole engine is Data oriented & Data driven.

All data structures are small struct with just the data needed for the task it's suppose to do.
They live inside "manager" who create, destroy and manage those struct.
All objects in a manager are treatd in batch, and layed out contiguously in memory, minimizing cache miss to a minimum

Also it allow to design object throught a single ID that can be easily passed around & remain valid even if objects are moved in memory, as long as their
ordering is preserved. Serializing a system is ten trivial, as it just mean dumping the system on disk, adn reloading it. All objects references will remain valid.

Coding style
============

All code is made throught the "Back To C" style.

This style of coding aim at keeping away all the pitfall and dark side of c++ by avoid over encapsulation, obscure templating 
and meta programming. By putting too much encapsulation barrier throught public/protectd/private access, people end up spending their time shifting
things around to access them anyway. People working at that level are knowledgeable enough (or should be) to know to access only what they should.

It doesn't mean guideline aren't offered. All variables starting with a _ mean that this variable is considered "private" and you should avoid
modifying it directly, apart if you really know what your doing. Prefer using the associated functions.

The style is much more closer to a functional programming one, with data structure on one hand, and function acting on those on a namespace with a 
similar name, usually taking the object as first parameter.

This doesn't mean template and inheritance shouldn't be used, but solely for specialization purpose (generic container) & data layout extension.
Virtual inheritance should be banned at all cost. This should be confined to the gameplay layer, either throught script or a specific part of the
engine using the c++ virtual inheritance paradigm.

Also it should be refrain to write preprocessor or template that generate automatically part of code, as they make the code obscure, hard to debug, and longer to compile.
It's better to loose 10s copying something, that loosing 2hr debugging an obscure pre-processor generating a whole implementation in a class.

GFX Abstraction
===============

The GFX abstraction layer work as follow : 

- Generic functions & struct describing all parameter shared through all gfx api (etc width/height of a texture, path of a shader etc...)
- Function pointers call nameImpl thashould be filled by the specific implementation (createShaderImpl should for exemple be filled with a function creating the actual D3D11 shader object)
- Specific struct inside each impl that contain specific data to that graphic API (ex in D3D11, a GFXVertexShaderD3D11 would contain the IDirect3DVertexShader pointer)

All those are stocked in two manager : 
- the Generic Manager for each type, that got generic structure info. This is the one accessed mostly by the engine user, as she don't have to bother with specific api object
- the Specific Manager, that got structure containing API specific data. This is mostly used by internal implementation functions to actually act on those.