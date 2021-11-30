import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SetRoomComponent } from './set-room.component';

describe('SetRoomComponent', () => {
  let component: SetRoomComponent;
  let fixture: ComponentFixture<SetRoomComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ SetRoomComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(SetRoomComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
