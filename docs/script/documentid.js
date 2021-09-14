export default class DocId {
	constructor(id) {
		let ids = id.split('.');
		this.document = ids[0];
		this.section = ids[1];
		this.subsection = ids[2];
	}

	getSectionId() 
	{
		return this.document + '.' + this.section;
	}

	getSubSectionId() 
	{
		return this.getSectionId() + '.' + this.subsection;
	}

	firstInSection()
	{
		return this.getSectionId() + '.0';
	}

	sameSection(docId)
	{
		return this.getSectionId == docId.getSectionId();
	}

}