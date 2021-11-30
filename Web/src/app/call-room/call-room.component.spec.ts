import { ComponentFixture, TestBed } from '@angular/core/testing';

import { CallRoomComponent } from './call-room.component';

describe('CallRoomComponent', () => {
  let component: CallRoomComponent;
  let fixture: ComponentFixture<CallRoomComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ CallRoomComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(CallRoomComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
