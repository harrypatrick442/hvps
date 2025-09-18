export default class Iterator{
	constructor(array)
	{
		this._index=0;
		this._array=array;
		this._length=array.length;
    }
	next()
    {
        return this._array[this._index++];
    }
    hasNext()
    {
      return this._index<this._length;
    }
    remove()
    {
        this._array.splice(--this._index, 1);
        this._length--;
    }
}